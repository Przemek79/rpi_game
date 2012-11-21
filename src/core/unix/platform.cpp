/*
 * Copyright 2011-2012 Marshmallow Platform. All rights reserved.
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

#include "core/platform.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <libgen.h>
#include <unistd.h>

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

/******************************************************************************/

namespace
{
	static time_t s_start_time = time(0);
} // namespace

/******************************************************************************/

void
Platform::Initialize(void)
{
	srand(static_cast<unsigned int>(s_start_time));
}

void
Platform::Finalize(void)
{
}

void
Platform::Sleep(MMTIME timeout)
{
	if (timeout <= 0) return;

	MMVERBOSE("Sleeping for " << timeout << " milliseconds.");

	struct timespec l_ts;
#define MILLISECONDS_PER_SECOND 1000
	l_ts.tv_sec = timeout / MILLISECONDS_PER_SECOND;
#define NANOSECONDS_PER_MILLISECOND 1000000
	l_ts.tv_nsec = static_cast<long int>
	    ((timeout % MILLISECONDS_PER_SECOND)
	        * NANOSECONDS_PER_MILLISECOND);
	nanosleep(&l_ts, 0);
}

time_t
Platform::StartTime(void)
{
	return(s_start_time);
}

MMTIME
Platform::TimeStamp(void)
{
	struct timeval time;
	gettimeofday(&time, 0);
	MMTIME out = static_cast<MMTIME>(((time.tv_sec - s_start_time) * 1000) + (time.tv_usec / 1000));
	return(out);
}

TimeData
Platform::TimeStampToTimeData(MMTIME timestamp)
{
	TimeData l_ts;
	struct tm l_time;

	l_ts.internal = timestamp;
	l_ts.system =
	    static_cast<time_t>(Platform::StartTime() +
	    static_cast<time_t>(l_ts.internal/1000));

	gmtime_r(&l_ts.system, &l_time);

	sprintf(l_ts.string, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		l_time.tm_year + 1900,
		l_time.tm_mon  + 1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return(l_ts);
}

std::string
Platform::PathDirectory(const std::string &path)
{
	char l_path[PATH_MAX + 1];

	l_path[PATH_MAX + 1] = 0;
	path.copy(l_path, PATH_MAX);

	return(dirname(l_path));
}

std::string
Platform::TemporaryDirectory(void)
{
	return("/tmp/");
}

