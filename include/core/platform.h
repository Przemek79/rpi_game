/*
 * Copyright 2011 Marshmallow Platform. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW PLATFORM ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW PLATFORM OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Platform.
 */

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H 1

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	/*! @brief Platform specific class */
	struct Platform
	{
		/*! @brief Platform internal structure */
		struct Internal;

		/*!
		 * @brief Platform specific initialization
		 */
		static void Initialize(void);

		/*!
		 * @brief Platform specific finalization
		 */
		static void Finalize(void);

		/* flow */

		static void PreRender(TIME timeout = INFINITE);
		static void PostRender(TIME timeout = INFINITE);
		static void PreSecond(TIME timeout = INFINITE);
		static void PostSecond(TIME timeout = INFINITE);
		static void PreTick(TIME timeout = INFINITE);
		static void PostTick(TIME timeout = INFINITE);
		static void PreUpdate(TIME timeout = INFINITE);
		static void PostUpdate(TIME timeout = INFINITE);

		/* time */

		/*!
		 * @brief Start system time
		 */
		static time_t StartTime(void);

		/*!
		 * @brief Milliseconds since StartTime()
		 */
		static TIME TimeStamp(void);
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
