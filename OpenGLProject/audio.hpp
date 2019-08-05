/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	audio.hpp
**
**	Summary		:	Audio system and sound resources.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <string>
#include <fmod/fmod.hpp>

#include "common.hpp"


constexpr int const numAudioChannels = 32;

extern FMOD::System* g_audio;

// sounds
extern FMOD::Sound* g_soundThump;
extern FMOD::Sound* g_soundZap;
extern FMOD::Sound* g_soundDing;

extern FMOD::Sound* g_musicBackground;



// functions

void setReasonFmod(FMOD_RESULT r, std::string const& message);

ReturnCode initAudio();
void destroyAudio();
ReturnCode updateAudio();

template<class T>
void destroyFmodPointer(T**const p)
{
	if (p && *p)
	{
		FMOD_RESULT const r = (**p).release();
		if (r)
		{
			*g_reason = "FMOD pointer release failed";
			printError(r);
		}

		*p = nullptr;
	}
}

ReturnCode loadSoundEffect(FMOD::Sound** out, std::string const& fileName);
ReturnCode loadMusicTrack(FMOD::Sound** out, std::string const& fileName);
