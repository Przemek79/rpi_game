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

#include "game/textcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/weak.h"

#include "math/point2.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/itileset.h"
#include "graphics/ivertexdata.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"

#include <tinyxml2.h>

#include <string>
#include <vector>

#define MIN_CHAR 33
#define MAX_CHAR 126

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

/******************************************************************************/

struct TextComponent::Private
{
	Private(void)
	: scale(1.f)
	, tile_offset(0)
	, invalidated(false) {}

	void rebuild(void);
	void render(void);

	std::vector<Graphics::SharedMesh> mesh;

	WeakPositionComponent position;
	Graphics::SharedTileset tileset;

	Graphics::Color color;

	std::string text;

	float scale;
	uint16_t tile_offset;
	bool invalidated;
};

void
TextComponent::Private::rebuild(void)
{
	mesh.clear();
	mesh.resize(text.size());

	if (!tileset) {
		MMWARNING("No tileset assigned.");
		return;
	}

	/* create vertex data */

	/* TODO: this needs to be kept around, only replaced when font size
	 *       changes.
	 */
	Graphics::SharedVertexData l_vdata =
	    Graphics::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
	{
		float l_hwidth  =
		    (static_cast<float>(tileset->tileSize().width)  / 2.f) * scale;
		float l_hheight =
		    (static_cast<float>(tileset->tileSize().height) / 2.f) * scale;

		l_vdata->set(0, -l_hwidth,  l_hheight);
		l_vdata->set(1, -l_hwidth, -l_hheight);
		l_vdata->set(2,  l_hwidth,  l_hheight);
		l_vdata->set(3,  l_hwidth, -l_hheight);
	}

	/* render characters */

	/* TODO: find line-breaks to determine line length for center
	 *       alignment, also add right alignment.
	 */
	char l_char;
	const size_t l_text_count = text.size();
	for (uint16_t i = 0; i < l_text_count; ++i) {
		l_char = text[i];
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::SharedTextureCoordinateData l_tdata =
				tileset->getTextureCoordinateData(static_cast<uint16_t>
				    (tile_offset + (l_char - MIN_CHAR)));

			mesh[i] = new Graphics::QuadMesh(l_tdata, tileset->textureData(), l_vdata);
		}
	}

	invalidated = false;
}

void
TextComponent::Private::render(void)
{
	if (invalidated) return;

	/* if no position component, abort! */
	if (!position) {
		MMWARNING("No position component found!");
		return;
	}

	/* render characters */

	/* TODO: find line-breaks to determine line length for center
	 *       alignment, also add right alignment.
	 */
	char l_char;
	Math::Point2 l_point(position->position());
	const size_t l_text_count = text.size();
	for (unsigned int i = 0; i < l_text_count; ++i) {
		l_char = text[i];

		/* render valid characters */
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::SharedQuadMesh l_mesh = mesh[i].staticCast<Graphics::QuadMesh>();
			l_mesh->setColor(color);
			Graphics::Painter::Draw(*l_mesh, l_point);
			l_point.x += static_cast<float>(tileset->tileSize().width) * scale;
		}

		/* handle line break */
		else if ('\n' == l_char) {
			l_point.x  = position->position().x;
			l_point.y += static_cast<float>(tileset->tileSize().height) * -scale;
		}

		/* skip unknown character */
		else l_point.x += static_cast<float>(tileset->tileSize().width) * scale;
	}
}

/******************************************************************************/

TextComponent::TextComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(new Private)
{
}

TextComponent::~TextComponent(void)
{
	delete m_p, m_p = 0;
}

Graphics::SharedTileset &
TextComponent::tileset(void)
{
	return(m_p->tileset);
}

const std::string &
TextComponent::text(void) const
{
	return(m_p->text);
}

const Graphics::Color &
TextComponent::color(void) const
{
	return(m_p->color);
}

void
TextComponent::setText(const std::string &t)
{
	m_p->text = t;
	m_p->invalidated = true;
	m_p->rebuild();
}

void
TextComponent::setColor(const Graphics::Color &c)
{
	m_p->color = c;
}

float
TextComponent::scale(void) const
{
	return(m_p->scale);
}

void
TextComponent::setScale(float s)
{
	m_p->scale = s;
	m_p->invalidated = true;
	m_p->rebuild();
}

uint16_t
TextComponent::tileOffset(void) const
{
	return(m_p->tile_offset);
}

void
TextComponent::setTileOffset(uint16_t o)
{
	m_p->tile_offset = o;
	m_p->invalidated = true;
	m_p->rebuild();
}

bool
TextComponent::serialize(XMLElement &n) const
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(false);
}

bool
TextComponent::deserialize(XMLElement &n)
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(false);
}

void
TextComponent::update(float delta)
{
	ComponentBase::update(delta);

	if (!m_p->position)
	    m_p->position = entity().getComponentType("Game::PositionComponent").
	        staticCast<PositionComponent>();

	if (m_p->invalidated)
	    m_p->rebuild();
}

void
TextComponent::render(void)
{
	ComponentBase::render();
	m_p->render();
}

const Core::Type &
TextComponent::Type(void)
{
	static const Core::Type s_type("Game::TextComponent");
	return(s_type);
}

