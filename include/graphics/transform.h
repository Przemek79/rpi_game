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

#ifndef MARSHMALLOW_GRAPHICS_TRANSFORM_H
#define MARSHMALLOW_GRAPHICS_TRANSFORM_H 1

#include <core/fd.h>
#include <core/iserializable.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	class Matrix4;
	struct Point2;
	template <typename T> struct Size2;
	typedef Size2<float> Size2f;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	/*! @brief Graphics Transform */
	class MARSHMALLOW_GRAPHICS_EXPORT
	Transform
	{
		struct Private;
		Private *m_p;
	public:
		enum MatrixType
		{
			mtModel,
			mtView
		};
	public:
		Transform(void);
		Transform(const Transform &other);
		virtual ~Transform(void);
		
		Transform & operator =(const Transform& rhs);

		float rotation(void) const;
		void setRotation(float value);

		const Math::Size2f & scale(void) const;
		void setScale(const Math::Size2f &value);
		void setScale(float w, float h);

		const Math::Point2 & translation(void) const;
		void setTranslation(const Math::Point2 &value);
		void setTranslation(float x, float y);

		const Math::Matrix4 & matrix(MatrixType type = mtModel) const;
	};
	typedef Core::Shared<Transform> SharedTransform;
	typedef Core::Weak<Transform> WeakTransform;

} /********************************************** Graphics::Painter Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
