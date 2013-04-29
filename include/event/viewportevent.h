/*
 * Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#ifndef MARSHMALLOW_EVENT_VIEWPORTEVENT_H
#define MARSHMALLOW_EVENT_VIEWPORTEVENT_H 1

#include <event/event.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Event { /****************************************** Event Namespace */

	/*! @brief Event Viewport Class */
	class MARSHMALLOW_EVENT_EXPORT
	ViewportEvent : public Event
	{
		PRIVATE_IMPLEMENTATION
		NO_ASSIGN_COPY(ViewportEvent);
	public:

		enum Reason
		{
			Created,
			Destroyed
		};

	public:

		ViewportEvent(Reason reason);
		virtual ~ViewportEvent(void);

		Reason reason(void) const;

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	public: /* static */

		static const Core::Type & Type(void);
	};

} /********************************************************** Event Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
