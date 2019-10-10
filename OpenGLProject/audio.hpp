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
#include <cress/moo/final_act.hpp>

#include "common.hpp"


constexpr int const numAudioChannels = 32;

extern FMOD::System* g_audio;

// sounds
extern FMOD::Sound* g_soundThump;
extern FMOD::Sound* g_soundZap;
extern FMOD::Sound* g_soundDing;

// R
extern FMOD::Sound* g_musicMenuBackground;
extern FMOD::Sound* g_musicGameBackground;
extern FMOD::Sound* g_soundNavigate;
extern FMOD::Sound* g_soundSelect;
extern FMOD::Sound* g_soundMovePiece;
extern FMOD::Sound* g_soundCapture;


// functions

void initAudio();
void destroyAudio();
void updateAudio();

template<class T>
void destroyFmodPointer(T*& p)
{
	CRESS_MOO_FINAL_ACT_SINGLE(fa, p = nullptr);
	if (p)
	{
		FMOD_RESULT const r = p->release();
		if (r) { throw std::runtime_error("FMOD pointer release failed: " + std::to_string(r)); }
	}
}

FMOD::Sound* loadSoundEffect(std::string const& fileName);
FMOD::Sound* loadMusicTrack(std::string const& fileName);
