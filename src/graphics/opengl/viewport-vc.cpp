/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Engine.
 */

#include "graphics/viewport.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

#include "graphics/camera.h"
#include "graphics/painter.h"

#include <bcm_host.h>

#include "headers.h"
#ifdef MARSHMALLOW_OPENGL_ES2_EGL
#  include <EGL/egl.h>
#else
#  error EGL/GLES2 is disabled!
#endif
#include "extensions.h"

/*
 * VC Viewport Notes
 *
 * Fullscreen flag is ignored since we will always be fullscreen.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

struct ViewportData
{
	DISPMANX_DISPLAY_HANDLE_T dpy;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_UPDATE_HANDLE_T  dispman_update;
	DISPLAY_INPUT_FORMAT_T    input_format;
	Math::Size2i wsize;
	int          screen;

	EGL_DISPMANX_WINDOW_T window;
	EGLContext  egl_ctx;
	EGLDisplay  egl_dpy;
	EGLSurface  egl_surface;

	Math::Size2f size;
	bool         loaded;
} s_data;

void
ResetViewportData(void)
{
	s_data.egl_ctx     = EGL_NO_CONTEXT;
	s_data.egl_dpy     = EGL_NO_DISPLAY;
	s_data.egl_surface = EGL_NO_SURFACE;

	s_data.dpy = 0;
	s_data.dispman_element = 0;
	s_data.dispman_update = 0;
	s_data.screen = 0;
	s_data.size.zero();
	s_data.wsize[0] = s_data.wsize[1] = 0;

	s_data.loaded = false;
}

bool GLCreateSurface(uint8_t depth, bool vsync);
bool VCCreateDisplay(uint16_t w, uint16_t h, uint8_t refresh);
bool VCPowerOnDisplay(uint8_t refresh, uint16_t &display_id);

bool
CreateDisplay(uint16_t w, uint16_t h, uint8_t depth, uint8_t refresh, bool vsync)
{
	using namespace Graphics;

	ResetViewportData();

	/* display */

	if (!VCCreateDisplay(w, h, refresh)) {
		MMERROR("VC: Failed to create display.");
		return(false);
	}

	/* context */

	if (!GLCreateSurface(depth, vsync)) {
		MMERROR("GL: Failed to create surface.");
		return(false);
	}

	/* initialize context */

	glViewport(0, 0, s_data.wsize[0], s_data.wsize[1]);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL: Failed during initialization.");
		return(false);
	}

	/* set viewport size */

#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
	s_data.size[0] = MARSHMALLOW_VIEWPORT_WIDTH;
	s_data.size[1] = static_cast<float>(s_data.wsize[1]) *
	    (MARSHMALLOW_VIEWPORT_WIDTH / static_cast<float>(s_data.wsize[0]));
#else
	s_data.size[0] = static_cast<float>(s_data.wsize[0]) *
	    (MARSHMALLOW_VIEWPORT_HEIGHT / static_cast<float>(s_data.wsize[1]));
	s_data.size[1] = MARSHMALLOW_VIEWPORT_HEIGHT;
#endif

	Camera::Update();

	return(s_data.loaded = true);
}

void
DestroyDisplay(void)
{
	if (s_data.dpy) {
		eglMakeCurrent(s_data.egl_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(s_data.egl_dpy, s_data.egl_ctx);
		eglDestroySurface(s_data.egl_dpy, s_data.egl_surface);
		eglTerminate(s_data.egl_dpy);

		if (s_data.dpy > 0) {
			vc_dispmanx_element_remove(s_data.dispman_update, s_data.dispman_element);
			vc_dispmanx_display_close(s_data.dpy);
		}
	}

	vc_tv_power_off();

	s_data.egl_ctx     = EGL_NO_CONTEXT;
	s_data.egl_dpy     = EGL_NO_DISPLAY;
	s_data.egl_surface = EGL_NO_SURFACE;

	s_data.dpy    = 0;
	s_data.loaded = false;
	s_data.screen = 0;
}

bool
GLCreateSurface(uint8_t depth, bool vsync)
{
	using namespace Graphics;

	s_data.egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (s_data.egl_dpy == EGL_NO_DISPLAY) {
		MMERROR("EGL: No display was found. EGLERROR=" << eglGetError());
		return(false);
	}
	
	if (!eglInitialize(s_data.egl_dpy, 0, 0)) {
		MMERROR("EGL: Initialization failed.");
		return(false);
	}

	/* color sizes */
	EGLint l_color_sizes[3];
	switch (s_data.input_format) {
	case VCOS_DISPLAY_INPUT_FORMAT_RGB888:
		l_color_sizes[0] = l_color_sizes[1] = l_color_sizes [2] = 8;
		break;
	case VCOS_DISPLAY_INPUT_FORMAT_RGB565:
		l_color_sizes[1] = 6;
		l_color_sizes[0] = l_color_sizes[2] = 5;
		break;
	default:
		l_color_sizes[0] = l_color_sizes[1] = l_color_sizes [2] = 0;
	}

	EGLint l_attr[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
		EGL_BUFFER_SIZE,     depth,
		EGL_DEPTH_SIZE,      0,
		EGL_RED_SIZE,        l_color_sizes[0],
		EGL_GREEN_SIZE,      l_color_sizes[1],
		EGL_BLUE_SIZE,       l_color_sizes[2],
		EGL_ALPHA_SIZE,      0,
		EGL_NONE,            EGL_NONE
	};

	EGLint l_config_count;
	EGLConfig l_config;
	if (!eglChooseConfig(s_data.egl_dpy, l_attr, &l_config, 1, &l_config_count))
		MMERROR("EGL: No config was chosen. EGLERROR=" << eglGetError());

	s_data.egl_surface =
	    eglCreateWindowSurface(s_data.egl_dpy, l_config,
	        (EGLNativeWindowType) &s_data.window, 0);
	if (s_data.egl_surface == EGL_NO_SURFACE) {
		MMERROR("EGL: No surface was created. EGLERROR=" << eglGetError());
		return(false);
	}

	EGLint l_ctxattr[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE, EGL_NONE
	};
	s_data.egl_ctx = eglCreateContext(s_data.egl_dpy, l_config, EGL_NO_CONTEXT, l_ctxattr);
	if (s_data.egl_ctx == EGL_NO_CONTEXT) {
		MMERROR("EGL: No context was created. EGLERROR=" << eglGetError());
		return(false);
	}
	
	if (!eglMakeCurrent(s_data.egl_dpy, s_data.egl_surface, s_data.egl_surface, s_data.egl_ctx)) {
		MMERROR("EGL: Failed to switch current context. EGLERROR=" << eglGetError());
		return(false);
	}

	/* extensions */

	OpenGL::InitializeExtensions(eglQueryString(s_data.egl_dpy, EGL_EXTENSIONS));

	/* vsync */

	eglSwapInterval(s_data.egl_dpy, vsync ? 1 : 0);

	return(true);
}

bool
VCCreateDisplay(uint16_t w, uint16_t h, uint8_t refresh)
{
	VC_RECT_T l_dst_rect;
	VC_RECT_T l_src_rect;

	s_data.wsize[0] = w;
	s_data.wsize[1] = h;

	l_src_rect.x = l_src_rect.y = 0;
	l_dst_rect.x = l_dst_rect.y = 0;
	l_dst_rect.width  = w;
	l_dst_rect.height = h;

	/* set SPD type to game */
	vc_tv_hdmi_set_spd("MMGE", "MES", HDMI_SPD_TYPE_GAME);

	uint16_t l_display_type;
	if (!VCPowerOnDisplay(refresh, l_display_type)) {
		MMERROR("VC: Failed to power on display.");
		return(false);
	}

	s_data.dpy = vc_dispmanx_display_open(l_display_type);
	if (s_data.dpy == 0) {
		MMERROR("VC: Failed to open display.");
		return(false);
	}

	DISPMANX_MODEINFO_T l_display_info;
	if (vc_dispmanx_display_get_info(s_data.dpy, &l_display_info) != 0) {
		MMERROR("VC: Failed to get display info.");
		return(false);
	}
	/* recalculate sizes */
	s_data.wsize[0]   = l_dst_rect.width  = l_display_info.width;
	s_data.wsize[1]   = l_dst_rect.height = l_display_info.height;
	l_src_rect.width  = l_dst_rect.width  << 16;
	l_src_rect.height = l_dst_rect.height << 16;
	MMINFO("VC: Display size (" << s_data.wsize[0] << "x" << s_data.wsize[1] << ")");

	s_data.dispman_update  = vc_dispmanx_update_start(0);
	s_data.dispman_element = vc_dispmanx_element_add
	    (s_data.dispman_update,
	     s_data.dpy,
	     0 /*layer*/,
	    &l_dst_rect,
	     0 /*src*/,
	    &l_src_rect,
	     DISPMANX_PROTECTION_NONE,
	     0 /*alpha*/,
	     0 /*clamp*/,
	     DISPMANX_NO_ROTATE /*transform*/);

	s_data.window.element = s_data.dispman_element;
	s_data.window.width   = l_dst_rect.width;
	s_data.window.height  = l_dst_rect.height;
	s_data.input_format   = l_display_info.input_format;
	vc_dispmanx_update_submit_sync(s_data.dispman_update);

	return(true);
}

bool
VCPowerOnDisplay(uint8_t refresh, uint16_t &did)
{
	uint16_t l_display = DISPMANX_ID_SDTV; /* default to tv output */

	/*
	 * Power on SDTV (needed for state testing)
	 */
	if (vc_tv_sdtv_power_on(SDTV_MODE_NTSC, 0) != 0) {
		MMERROR("SDTV: Failed to power on.");
		return(false);
	}

	TV_GET_STATE_RESP_T l_tvstate;
	if (vc_tv_get_state(&l_tvstate) != 0) {
		MMERROR("HDMI: Failed to query initial tv state.");
		return(false);
	}

	MMINFO("VC: state=" << l_tvstate.state);

	/*
	 *  HDMI/DVI
	 */
	if ((l_tvstate.state & VC_HDMI_UNPLUGGED) == 0) {

		/* switch to stand-by */
		if ((l_tvstate.state & VC_HDMI_STANDBY) == 0)
			vc_tv_power_off();

		/* power on in correct mode */
		if (vc_tv_hdmi_power_on_best(0, 0, refresh, HDMI_NONINTERLACED,
		    HDMI_MODE_MATCH_FRAMERATE) != 0) {
			MMERROR("HDMI: Failed to power on.");
			return(false);
		}

		/* update state */
		if (vc_tv_get_state(&l_tvstate) != 0) {
			MMERROR("HDMI: Failed to query tv state after HDMI power on.");
			return(false);
		}

		/* check connection type */
		if ((l_tvstate.state & VC_HDMI_HDMI) == VC_HDMI_HDMI)
			l_display = DISPMANX_ID_HDMI;
		else if ((l_tvstate.state & VC_HDMI_DVI) == VC_HDMI_DVI)
			l_display = DISPMANX_ID_MAIN_LCD;

	}

	if (l_tvstate.width <= 0 || l_tvstate.height <= 0) {
		MMERROR("VC: Invalid TV size returned.");
		return(false);
	}

	did = l_display;
	return(true);
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	return(eglGetProcAddress(f));
}

/********************************************************* Graphics::Viewport */

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t depth, uint8_t refresh,
                     bool, bool vsync)
{
	bcm_host_init();

	Camera::Reset();

	if (!CreateDisplay(w, h, depth, refresh, vsync)) {
		DestroyDisplay();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();

	DestroyDisplay();

	bcm_host_deinit();
}

bool
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t depth, uint8_t refresh,
                    bool, bool vsync)
{
	DestroyDisplay();

	if (!CreateDisplay(w, h, depth, refresh, vsync)) {
		DestroyDisplay();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	/* TODO */
}

void
Viewport::SwapBuffer(void)
{
	eglSwapBuffers(s_data.egl_dpy, s_data.egl_surface);
	Painter::Reset();
}

const Math::Size2f &
Viewport::Size(void)
{
	return(s_data.size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	return(s_data.wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("VC");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

