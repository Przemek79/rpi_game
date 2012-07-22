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

#include "graphics/camera.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

bool
Viewport::Initialize(uint16_t, uint16_t, uint8_t, uint8_t, bool, bool)
{
	Camera::Reset();

	MMDEBUG("Dummy viewport initialized.");

	Camera::Update();

#if MARSHMALLOW_DUMMY_FAIL
	return(false);
#else
	return(true);
#endif
}

void
Viewport::Finalize(void)
{
	MMDEBUG("Dummy viewport finalized.");
}

bool
Viewport::Redisplay(uint16_t, uint16_t, uint8_t, uint8_t, bool, bool)
{
	MMDEBUG("Dummy viewport re-initialized.");

	Camera::Update();

#if MARSHMALLOW_DUMMY_FAIL
	return(false);
#else
	return(true);
#endif
}

void
Viewport::Tick(void)
{
}

void
Viewport::SwapBuffer(void)
{
}

const Math::Size2f &
Viewport::Size(void)
{
	const static Math::Size2f s_size(MARSHMALLOW_VIEWPORT_WIDTH,
	                                 MARSHMALLOW_VIEWPORT_HEIGHT);
	return(s_size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	const static Math::Size2i s_wsize(MARSHMALLOW_VIEWPORT_WIDTH,
	                                  MARSHMALLOW_VIEWPORT_HEIGHT);
	return(s_wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("DUMMY");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

