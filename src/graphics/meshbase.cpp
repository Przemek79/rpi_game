/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "graphics/meshbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml2.h>

#include "core/logger.h"
#include "core/shared.h"
#include "core/type.h"

#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/ivertexdata.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace { /******************************** Graphics::<anonymous> Namespace */

static const char * s_scalemode_string[Graphics::ITextureData::smModes] =
    { "nearest", "linear" };

Graphics::ITextureData::ScaleMode
StringToScaleMode(const char *str)
{
	using namespace Graphics;

	if (!str) return(ITextureData::smDefault);

	for (int i = 0; i < ITextureData::smModes; ++i)
		if (MMSTRCASECMP(str, s_scalemode_string[i]) == 0)
			return(static_cast<ITextureData::ScaleMode>(i));

	return(ITextureData::smDefault);
}

const char *
ScaleModeToString(Graphics::ITextureData::ScaleMode mode)
{
	return(s_scalemode_string[mode]);
}

} /****************************************** Graphics::<anonymous> Namespace */

struct MeshBase::Private
{
	SharedTextureCoordinateData tcdata;
	SharedTextureData tdata;
	SharedVertexData vdata;
	Color color;
	float scale[2];
	float rotation;
};

MeshBase::MeshBase(SharedTextureCoordinateData tc, SharedTextureData t, SharedVertexData v)
    : m_p(new Private)
{
	m_p->tcdata = tc;
	m_p->tdata = t;
	m_p->vdata = v;
	m_p->rotation = 0;
	m_p->scale[0] = m_p->scale[1] = 1.f;
}

MeshBase::~MeshBase(void)
{
	delete m_p, m_p = 0;
}

void
MeshBase::setColor(const Graphics::Color &c)
{
	m_p->color = c;
}

void
MeshBase::setRotation(float a)
{
	m_p->rotation = a;
}

void
MeshBase::setScale(float x, float y)
{
	m_p->scale[0] = x;
	m_p->scale[1] = y;
}

void
MeshBase::setTextureCoordinateData(SharedTextureCoordinateData tc)
{
	m_p->tcdata = tc;
}

void
MeshBase::setTextureData(SharedTextureData td)
{
	m_p->tdata = td;
}

void
MeshBase::setVertexData(Graphics::SharedVertexData vd)
{
	m_p->vdata = vd;
}

const Graphics::SharedTextureCoordinateData &
MeshBase::textureCoordinateData(void) const
{
	return(m_p->tcdata);
}

const Graphics::SharedTextureData &
MeshBase::textureData(void) const
{
	return(m_p->tdata);
}

const Graphics::SharedVertexData &
MeshBase::vertexData(void) const
{
	return(m_p->vdata);
}

const Graphics::Color &
MeshBase::color(void) const
{
	return(m_p->color);
}

float
MeshBase::rotation(void) const
{
	return(m_p->rotation);
}

void
MeshBase::scale(float &x, float &y) const
{
	x = m_p->scale[0];
	y = m_p->scale[1];
}

Math::Vector2
MeshBase::vertex(uint16_t i) const
{
	Math::Vector2 l_vector;
	if (!m_p->vdata->get(i, l_vector.x, l_vector.y))
		MMWARNING("Failed to retrieve values for vertex " << i);
	return(l_vector);
}

void
MeshBase::setVertex(uint16_t i, const Math::Vector2 &v)
{
	if (!m_p->vdata->set(i, v.x, v.y))
		MMWARNING("Failed to assign values (" << v.x << ", " << v.y << ") to vertex " << i);
}

void
MeshBase::textureCoordinate(uint16_t i, float &u, float &v) const
{
	if (!m_p->tcdata->get(i, u, v))
		MMWARNING("Failed to retrieve values for vertex " << i);
}

void
MeshBase::setTextureCoordinate(uint16_t i, float u, float v)
{
	if (!m_p->tcdata->set(i, u, v))
		MMWARNING("Failed to assign values (" << u << ", " << v << ") to texture coordinate " << i);
}

bool
MeshBase::serialize(XMLElement &n) const
{
	n.SetAttribute("type", type().str().c_str());
	n.SetAttribute("rotation", m_p->rotation);

	/* color */
	XMLElement *l_color = n.GetDocument()->NewElement("color");
	l_color->SetAttribute("r", m_p->color[0]);
	l_color->SetAttribute("g", m_p->color[1]);
	l_color->SetAttribute("b", m_p->color[2]);
	l_color->SetAttribute("a", m_p->color[3]);
	n.InsertEndChild(l_color);

	/* texture */
	if (m_p->tdata->isLoaded()) {
		XMLElement *l_texture = n.GetDocument()->NewElement("texture");
		l_texture->SetAttribute("id", m_p->tdata->id().str().c_str());
		l_texture->SetAttribute("min", ScaleModeToString(m_p->tdata->minificationMode()));
		l_texture->SetAttribute("mag", ScaleModeToString(m_p->tdata->magnificationMode()));
		n.InsertEndChild(l_texture);
	}

	/* texture coordinates */
	for (uint16_t i = 0; i < m_p->tcdata->count(); ++i) {
		float l_u, l_v;
		if (m_p->tcdata->get(i, l_u, l_v)) {
			XMLElement *l_vector = n.GetDocument()->NewElement("tcoord");
			l_vector->SetAttribute("u", l_u);
			l_vector->SetAttribute("v", l_v);
			n.InsertEndChild(l_vector);
		} else MMWARNING("Failed to serialize text coord " << i);
	}

	/* vertexes */
	for (uint16_t i = 0; i < m_p->vdata->count(); ++i) {
		float l_x, l_y;
		if (m_p->vdata->get(i, l_x, l_y)) {
			XMLElement *l_vector = n.GetDocument()->NewElement("vector");
			l_vector->SetAttribute("x", l_x);
			l_vector->SetAttribute("y", l_y);
			n.InsertEndChild(l_vector);
		} else MMWARNING("Failed to serialize vertex " << i);
	}

	return(true);
}

bool
MeshBase::deserialize(XMLElement &n)
{
	XMLElement *l_child;
	uint16_t l_i;

	n.QueryFloatAttribute("rotation", &m_p->rotation);

	/* color */
	l_child = n.FirstChildElement("color");
	if (l_child) {
		l_child->QueryFloatAttribute("r", &m_p->color[0]);
		l_child->QueryFloatAttribute("g", &m_p->color[1]);
		l_child->QueryFloatAttribute("b", &m_p->color[2]);
		l_child->QueryFloatAttribute("a", &m_p->color[3]);
	}

	/* scale */
	l_child = n.FirstChildElement("scale");
	if (l_child) {
		l_child->QueryFloatAttribute("x", &m_p->scale[0]);
		l_child->QueryFloatAttribute("y", &m_p->scale[1]);
	}

	/* texture */
	l_child = n.FirstChildElement("texture");
	if (l_child) {
		const char *l_file = l_child->Attribute("id");
		if (l_file) {
			const ITextureData::ScaleMode l_mag =
			    StringToScaleMode(l_child->Attribute("mag"));
			const ITextureData::ScaleMode l_min =
			    StringToScaleMode(l_child->Attribute("min"));
			m_p->tdata->load(l_file, l_mag, l_min);
		}
	}

	/* texture coordinates */
	for (l_child = n.FirstChildElement("tcoord"), l_i = 0;
	     l_child;
	     l_child = l_child->NextSiblingElement("tcoord")) {
		float l_u, l_v;
		l_child->QueryFloatAttribute("u", &l_u);
		l_child->QueryFloatAttribute("v", &l_v);
		if (!m_p->tcdata->set(l_i++, l_u, l_v)) {
			MMWARNING("Failed to assign texture coordinate data.");
			break;
		}
	}

	/* vertexes */
	for (l_child = n.FirstChildElement("vector"), l_i = 0;
	     l_child;
	     l_child = l_child->NextSiblingElement("vector")) {
		float l_x, l_y;
		l_child->QueryFloatAttribute("x", &l_x);
		l_child->QueryFloatAttribute("y", &l_y);
		if (!m_p->vdata->set(l_i++, l_x, l_y)) {
			MMWARNING("Failed to assign vertex data.");
			break;
		}
	}

	return(true);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

