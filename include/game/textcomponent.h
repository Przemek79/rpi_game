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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GAME_TEXTCOMPONENT_H
#define MARSHMALLOW_GAME_TEXTCOMPONENT_H 1

#include <core/fd.h>

#include <game/componentbase.h>

#include <string>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
	struct ITileset;
	typedef Core::Shared<ITileset> SharedTileset;

	struct IMesh;
	typedef Core::Shared<IMesh> SharedMesh;

	class Color;
} /******************************************************* Graphics Namespace */

namespace Game { /******************************************** Game Namespace */

	class PositionComponent;
	typedef Core::Weak<PositionComponent> WeakPositionComponent;

	/*! @brief Game Text Component Class */
	class MARSHMALLOW_GAME_EXPORT
	TextComponent : public ComponentBase
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(TextComponent);
	public:

		TextComponent(const Core::Identifier &i, IEntity &entity);
		virtual ~TextComponent(void);

		Graphics::SharedTileset & tileset(void);

		const std::string & text(void) const;
		void setText(const std::string &text);

		const Graphics::Color & color(void) const;
		void setColor(const Graphics::Color &color);

		float scale(void) const;
		void setScale(float scale);

		uint16_t tileOffset(void) const;
		void setTileOffset(uint16_t);

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void render(void);
		VIRTUAL void update(float d);

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);

	public: /* static */

		static const Core::Type & Type(void);
	};
	typedef Core::Shared<TextComponent> SharedTextComponent;
	typedef Core::Weak<TextComponent> WeakTextComponent;

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
