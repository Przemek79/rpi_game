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

#include "event/sensorevent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Event { /****************************************** Event Namespace */

struct SensorEvent::Private
{
	float x;
	float y;
	float z;
};

SensorEvent::SensorEvent(Input::Sensor::Type type_,
                       float x_, float y_, float z_,
                       size_t source_,
                       MMTIME timestamp_)
    : InputEvent(itSensor, type_, 0, source_, timestamp_)
    , PIMPL_CREATE
{
	PIMPL->x = x_;
	PIMPL->y = y_;
	PIMPL->z = z_;
}

SensorEvent::~SensorEvent(void)
{
	PIMPL_DESTROY;
}

float
SensorEvent::x() const
{
	return(PIMPL->x);
}

float
SensorEvent::y() const
{
	return(PIMPL->y);
}

float
SensorEvent::z() const
{
	return(PIMPL->z);
}

const Core::Type &
SensorEvent::Type(void)
{
	static const Core::Type s_type("Event::SensorEvent");
	return(s_type);
}

} /********************************************************** Event Namespace */
MARSHMALLOW_NAMESPACE_END

