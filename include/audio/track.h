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

#ifndef MARSHMALLOW_AUDIO_TRACK_H
#define MARSHMALLOW_AUDIO_TRACK_H 1

#include <core/iasset.h>

#include <core/global.h>
#include <core/fd.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */
	struct IDataIO;
	typedef Core::Shared<IDataIO> SharedDataIO;
} /*********************************************************** Core Namespace */

namespace Audio { /****************************************** Audio Namespace */

	class PCM;
	typedef Core::Weak<PCM> WeakPCM;

	struct ICodec;
	typedef Core::Shared<ICodec> SharedCodec;


	/*!
	 * @brief Audio Track
	 */
	class MARSHMALLOW_AUDIO_EXPORT
	Track
	{
		struct Private;
		Private *m_p;

	public:

		Track(void);
		Track(const Audio::WeakPCM &pcm, const Audio::SharedCodec &codec);
		~Track(void);

		const Audio::WeakPCM & pcm(void) const;
		void setPCM(const Audio::WeakPCM &pcm);

		const Audio::SharedCodec & codec(void) const;
		void setCodec(const Audio::SharedCodec &codec);

		bool play(int iterations = 1);
		void stop(void);
		bool isPlaying(void) const;

		void tick(float delta);

		bool isValid(void) const;
	};
	typedef Core::Shared<Track> SharedTrack;
	typedef Core::Weak<Track> WeakTrack;

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
