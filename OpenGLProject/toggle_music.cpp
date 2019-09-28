/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	toggle_music.cpp
**
**	Summary		:	Functions for playing, pausing, and stopping the background music.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "audio.hpp"

#include "toggle_music.hpp"



namespace
{
	FMOD::Channel* g_musicChannel = nullptr;
	bool g_isMusicPlaying = false;
}



bool isMusicPaused()
{
	bool b;
	FMOD_RESULT const r = g_musicChannel->getPaused(&b);
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::getPaused failed");
		return true;
	}
	return b;
}

ReturnCode setMusicPaused(bool paused)
{
	FMOD_RESULT const r = g_musicChannel->setPaused(paused);
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::setPaused failed");
		return RC_ERROR;
	}
	return RC_SUCCESS;
}

// starts the background music
ReturnCode playMusic()
{
	ASSERT0(g_isMusicPlaying);

	ASSERT1(g_audio);
	ASSERT1(g_musicMenuBackground);
	// Changed from g_musicBackground to g_musicGameBackground...
	// Currently Need a way to change the music based on if your at the main menu or in game...
	FMOD_RESULT r = g_audio->playSound(g_musicGameBackground, nullptr, true, &g_musicChannel);
	if (r)
	{
		setReasonFmod(r, "FMOD::System::playSound failed");
		return RC_ERROR;
	}

	g_isMusicPlaying = true;

	ASSERT1(g_musicChannel);

	r = g_musicChannel->setVolume(0.5f);
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::setVolume failed");
		return RC_ERROR;
	}

	ASSERT0(setMusicPaused(false));

	return RC_SUCCESS;
}

// stops the background music
ReturnCode stopMusic()
{
	ASSERT1(g_isMusicPlaying);
	ASSERT1(g_musicChannel);

	FMOD_RESULT const r = g_musicChannel->stop();
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::stop failed");
		return RC_ERROR;
	}

	g_isMusicPlaying = false;

	return RC_SUCCESS;
}

ReturnCode toggleMusicPaused()
{
	ASSERT1(g_musicChannel);

	bool isPaused;
	FMOD_RESULT r = g_musicChannel->getPaused(&isPaused);
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::getPaused failed");
		return RC_ERROR;
	}

	r = g_musicChannel->setPaused(!isPaused);
	if (r)
	{
		setReasonFmod(r, "FMOD::Channel::setPaused failed");
		return RC_ERROR;
	}

	return RC_SUCCESS;
}

bool isMusicPlaying()
{
	return g_isMusicPlaying;
}



ReturnCode playSoundEffect(FMOD::Sound* sound)
{
	if (!g_audio || !sound)
	{
		return RC_ERROR;
	}
	FMOD_RESULT const r = g_audio->playSound(sound, nullptr, false, nullptr);
	if (r)
	{
		setReasonFmod(r, "failed to play sound effect");
		return RC_ERROR;
	}
	return RC_SUCCESS;
}
