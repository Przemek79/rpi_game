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

#include "event/debugeventlistener.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <fstream>

#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

#include "event/ievent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Event { /****************************************** Event Namespace */

struct DebugEventListener::Private
{
	std::ofstream filestream;
};

DebugEventListener::DebugEventListener(const std::string &f)
    : m_p(new Private)
{
	m_p->filestream.open(f.c_str(), std::ios_base::app);
	if (m_p->filestream.is_open())
		m_p->filestream << std::hex;
	else MMERROR("Unable to open output file.");
}

DebugEventListener::~DebugEventListener(void)
{
	m_p->filestream.close();

	delete m_p, m_p = 0;
}

bool
DebugEventListener::handleEvent(const IEvent &e)
{
	if (m_p->filestream.is_open())
		m_p->filestream
		    << Core::Platform::TimeStampToTimeData(e.timeStamp()).string
		    << ": MS " << e.timeStamp()
		    << ": Event " << static_cast<const void *>(&e)
		    << ": Type (" << e.type().uid() << ")" << e.type().str().c_str()
		    << std::endl;

	return false;
}

} /********************************************************** Event Namespace */
MARSHMALLOW_NAMESPACE_END

