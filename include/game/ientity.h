/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef GAME_IENTITY_H
#define GAME_IENTITY_H 1

#include "core/irenderable.h"
#include "core/iserializable.h"
#include "core/iupdateable.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	class StrHash;
	typedef StrHash Identifier;
	typedef StrHash Type;

	template <class T> class Shared;
	template <class T> class Weak;
}

namespace Game
{
	struct IComponent;
	typedef Core::Shared<IComponent> SharedComponent;

	struct IScene;

	/*! @brief Game Entity Interface */
	struct GAME_EXPORT IEntity : public Core::IRenderable,
	                             public Core::IUpdateable,
	                             public Core::ISerializable
	{
		virtual ~IEntity(void) {};

		virtual const Core::Identifier & id(void) const = 0;
		virtual const Core::Type & type(void) const = 0;

		virtual IScene &scene(void) = 0;

		virtual void addComponent(SharedComponent component) = 0;
		virtual void removeComponent(const SharedComponent &component) = 0;
		virtual SharedComponent component(const Core::Identifier &identifier) const = 0;
		virtual SharedComponent componentType(const Core::Type &type) const = 0;

		virtual void kill(void) = 0;
		virtual bool isZombie(void) const = 0;
	};
	typedef Core::Shared<IEntity> SharedEntity;
	typedef Core::Weak<IEntity> WeakEntity;

}

MARSHMALLOW_NAMESPACE_END

#endif
