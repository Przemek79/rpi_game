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

#include "audio/player.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <map>

#include <cstring>

#include "core/identifier.h"
#include "core/logger.h"

#include "audio/itrack.h"
#include "audio/pcm.h"

#include "backend_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */
namespace { /*********************************** Audio::<anonymous> Namespace */

	static inline char
	Mixer(char a, char b, float gain)
	{
		return(char(MMRANGE(CHAR_MIN, a + int(b * gain), CHAR_MAX)));
	}

} /********************************************* Audio::<anonymous> Namespace */

struct Player::Private
{
	Private(void)
	{
	}

	~Private(void)
	{
	}

	inline void load(const Core::Identifier &id, ITrack *track);
	inline bool contains(const Core::Identifier &id);
	inline void eject(const Core::Identifier &id);

	inline bool play(const Core::Identifier &id, int playlist, float gain);
	inline void stop(const Core::Identifier &id);
	inline bool isPlaying(const Core::Identifier &id) const;

	typedef std::map<const Core::Identifier, ITrack *> TrackMap;
	TrackMap tracks;

	typedef std::pair<int, float> IterationGainPair;
	typedef std::map<const Core::Identifier, IterationGainPair> PlaylistMap;
	PlaylistMap playlist;

	void tick(void);

	PCM *pcm;
};

void
Player::Private::load(const Core::Identifier &id, ITrack *track)
{
	tracks[id] = track;
}

bool
Player::Private::contains(const Core::Identifier &id)
{
	return(tracks.find(id) != tracks.end());
}

void
Player::Private::eject(const Core::Identifier &id)
{
	TrackMap::iterator l_i = tracks.find(id);

	if (l_i != tracks.end())
		tracks.erase(l_i);
}

bool
Player::Private::play(const Core::Identifier &id, int iterations, float gain)
{
	if (0 == iterations) {
		stop(id);
		return(false);
	}
	else {
		playlist[id] = std::make_pair(iterations, gain);
		return(true);
	}
}

void
Player::Private::stop(const Core::Identifier &id)
{
	PlaylistMap::iterator l_i = playlist.find(id);
	if (l_i != playlist.end())
		playlist.erase(l_i);
}

bool
Player::Private::isPlaying(const Core::Identifier &id) const
{
	return(playlist.find(id) != playlist.end());
}

void
Player::Private::tick(void)
{
	if (!pcm) return;

	const size_t l_frames_available = pcm->framesAvailable();
	if (!l_frames_available)
		return;

	const size_t l_buffer_size = l_frames_available * pcm->frameSize();
	char *l_buffer = new char[l_buffer_size];
	char *l_mix = new char[l_buffer_size];

	memset(l_mix, 0, l_buffer_size);

	PlaylistMap::iterator l_i;
	PlaylistMap::const_iterator l_c = playlist.end();
	for (l_i = playlist.begin(); l_i != l_c;) {
		PlaylistMap::value_type l_track_i = *l_i;
		ITrack *l_track = tracks[l_track_i.first];

		size_t l_offset = 0;
		size_t l_read = 0;
		do {
			/* decode */
			l_read += l_track->read(&l_buffer[l_offset],
			    l_buffer_size - l_offset);

			/* mix */
			for (size_t l_bi = l_offset; l_bi < l_read; ++l_bi)
				l_mix[l_bi] =
				    Mixer(l_mix[l_bi],
				          l_buffer[l_bi],
				          l_track_i.second.second);

			/*
			 * Success! Next track.
			 */
			if (l_read == l_buffer_size)
				++l_i;

			/*
			 * Failed! Underrun occured.
			 */
	 		else {
				/* auto-rewind */
				l_track->rewind();

				/* if looping forever, continue */
				if (-1 == l_track_i.second.first)
					continue;

				/* check if we need to stop */
				else if (0 == --l_track_i.second.first) {
					playlist.erase(l_i++);
					break;
				}

				/* update playlist */
				else playlist[l_track_i.first] = l_track_i.second;
			}

			l_offset = l_read;
		}
		while(l_read < l_buffer_size);
	}
	
	if (!pcm->write(l_mix, l_frames_available))
		MMERROR("Failed to write to PCM device!");

	delete[] l_mix, l_mix = 0;
	delete[] l_buffer, l_buffer = 0;
}

/********************************************************************* Player */

Player::Player(void)
    : m_p(new Private)
{
}

Player::~Player(void)
{
	delete m_p, m_p = 0;
}

void
Player::load(const Core::Identifier &id, ITrack *track)
{
	m_p->load(id, track);
}

bool
Player::contains(const Core::Identifier &id)
{
	return(m_p->contains(id));
}

void
Player::eject(const Core::Identifier &id)
{
	m_p->eject(id);
}

bool
Player::play(const Core::Identifier &id, int playlist, float gain)
{
	return(m_p->play(id, playlist, gain));
}

void
Player::stop(const Core::Identifier &id)
{
	m_p->stop(id);
}

bool
Player::isPlaying(const Core::Identifier &id) const
{
	return(m_p->isPlaying(id));
}

void
Player::tick(void)
{
	m_p->tick();
}

PCM *
Player::pcm(void) const
{
	return(m_p->pcm);
}

void
Player::setPCM(PCM *_pcm)
{
	m_p->pcm = _pcm;
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END
