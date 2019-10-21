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
	FMOD::Channel* g_musicChannelG = nullptr;
	FMOD::Channel* g_musicChannelW = nullptr;
	bool g_isMusicPlaying = false;
	bool g_isMusicPlayingG = false;
	bool g_isMusicPlayingW = false;
}



bool isMusicPaused()
{
	assert(g_musicChannel);
	if (!g_musicChannel) { throw std::runtime_error("music channel is null"); }

	bool b;
	FMOD_RESULT const r = g_musicChannel->getPaused(&b);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::getPaused failed. FMOD_RESULT: " + std::to_string(r));
	}
	return b;
}

void setMusicPaused(bool paused)
{
	assert(g_musicChannel);
	if (!g_musicChannel) { throw std::runtime_error("music channel is null"); }

	FMOD_RESULT const r = g_musicChannel->setPaused(paused);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setPaused failed. FMOD_RESULT: " + std::to_string(r));
	}
}

void setMusicPausedG(bool paused)
{
	assert(g_musicChannelG);
	if (!g_musicChannelG) { throw std::runtime_error("game music channel is null"); }

	FMOD_RESULT const r = g_musicChannelG->setPaused(paused);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setPaused failed. FMOD_RESULT: " + std::to_string(r));
	}
}

void setMusicPausedW(bool paused)
{
	assert(g_musicChannelW);
	if (!g_musicChannelW) { throw std::runtime_error("game music channel is null"); }

	FMOD_RESULT const r = g_musicChannelW->setPaused(paused);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setPaused failed. FMOD_RESULT: " + std::to_string(r));
	}
}

// starts the background music
void playMusic()//FMOD_RESULT r)//= g_audio->playSound(FMOD::Sound **,nullptr, true,FMOD::Channel**))
{
	assert(!g_isMusicPlaying);
	if (g_isMusicPlaying) { throw std::runtime_error("music is already playing"); }

	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	assert(g_musicMenuBackground);
	if (!g_musicMenuBackground) { throw std::runtime_error("background music is null"); }

	// Changed from g_musicBackground to g_musicGameBackground...
	// Currently Need a way to change the music based on if your at the main menu or in game...
	FMOD_RESULT r = g_audio->playSound(g_musicMenuBackground, nullptr, true, &g_musicChannel);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::System::playSound failed. FMOD_RESULT: " + std::to_string(r));
	}

	g_isMusicPlaying = true;

	assert(g_musicChannel);
	if (!g_musicChannel) { throw std::runtime_error("music channel is null"); }

	r = g_musicChannel->setVolume(0.5f);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setVolume failed. FMOD_RESULT: " + std::to_string(r));
	}

	setMusicPaused(false);
}

// starts the background music
void playMusicG()//FMOD_RESULT r)//= g_audio->playSound(FMOD::Sound **,nullptr, true,FMOD::Channel**))
{
	assert(!g_isMusicPlayingG);
	if (g_isMusicPlayingG) { throw std::runtime_error("music is already playing"); }

	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	assert(g_musicGameBackground);
	if (!g_musicGameBackground) { throw std::runtime_error("game music is null"); }

	// Changed from g_musicBackground to g_musicGameBackground...
	// Currently Need a way to change the music based on if your at the main menu or in game...
	FMOD_RESULT r = g_audio->playSound(g_musicGameBackground, nullptr, true, &g_musicChannelG);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::System::playSound failed. FMOD_RESULT: " + std::to_string(r));
	}

	g_isMusicPlayingG = true;

	assert(g_musicChannelG);
	if (!g_musicChannelG) { throw std::runtime_error("game music channel is null"); }

	r = g_musicChannelG->setVolume(0.5f);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setVolume failed. FMOD_RESULT: " + std::to_string(r));
	}

	setMusicPausedG(false);
}

// starts the background music
void playMusicW()//FMOD_RESULT r)//= g_audio->playSound(FMOD::Sound **,nullptr, true,FMOD::Channel**))
{
	assert(!g_isMusicPlayingW);
	if (g_isMusicPlayingW) { throw std::runtime_error("music is already playing"); }

	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	// SET TO WIN AUDIO
	assert(g_musicWinBackground);
	if (!g_musicWinBackground) { throw std::runtime_error("win music is null"); }

	// Changed from g_musicBackground to g_musicGameBackground...
	// Currently Need a way to change the music based on if your at the main menu or in game...
	FMOD_RESULT r = g_audio->playSound(g_musicWinBackground, nullptr, true, &g_musicChannelW);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::System::playSound failed. FMOD_RESULT: " + std::to_string(r));
	}

	// Update
	g_isMusicPlayingW = true;

	assert(g_musicChannelW);
	if (!g_musicChannelW) { throw std::runtime_error("win music channel is null"); }

	r = g_musicChannelW->setVolume(0.5f);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setVolume failed. FMOD_RESULT: " + std::to_string(r));
	}

	setMusicPausedW(false);
}

// stops the background music
void stopMusic()
{
	if (!g_isMusicPlaying) { return; }
	//assert(g_isMusicPlaying);
	//if (!g_isMusicPlaying) { throw std::runtime_error("music is not currently playing"); }

	assert(g_musicChannel);
	if (!g_musicChannel) { throw std::runtime_error("music channel is null"); }

	FMOD_RESULT const r = g_musicChannel->stop();
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::stop failed. FMOD_RESULT: " + std::to_string(r));
	}

	g_musicChannel = nullptr;
	g_isMusicPlaying = false;
}

// stops the background music
void stopMusicG()
{
	if (!g_isMusicPlayingG) { return; }
	//assert(g_isMusicPlayingG);
	//if (!g_isMusicPlayingG) { throw std::runtime_error("music is not currently playing"); }

	assert(g_musicChannelG);
	if (!g_musicChannelG) { throw std::runtime_error("game music channel is null"); }

	FMOD_RESULT const r = g_musicChannelG->stop();
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::stop failed. FMOD_RESULT: " + std::to_string(r));
	}

	g_musicChannelG = nullptr;
	g_isMusicPlayingG = false;
}

// stops the background music
void stopMusicW()
{
	if (!g_isMusicPlayingW) { return; }
	//assert(g_isMusicPlayingG);
	//if (!g_isMusicPlayingG) { throw std::runtime_error("music is not currently playing"); }

	assert(g_musicChannelW);
	if (!g_musicChannelW) { throw std::runtime_error("game music channel is null"); }

	FMOD_RESULT const r = g_musicChannelW->stop();
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::stop failed. FMOD_RESULT: " + std::to_string(r));
	}

	g_musicChannelW = nullptr;
	g_isMusicPlayingW = false;
}


void toggleMusicPaused()
{
	assert(g_musicChannel);
	if (!g_musicChannel) { throw std::runtime_error("music channel is null"); }

	bool isPaused;
	FMOD_RESULT r = g_musicChannel->getPaused(&isPaused);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::getPaused failed. FMOD_RESULT: " + std::to_string(r));
	}

	r = g_musicChannel->setPaused(!isPaused);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::Channel::setPaused failed. FMOD_RESULT: " + std::to_string(r));
	}
}

bool isMusicPlaying()
{
	return g_isMusicPlaying;
}



void playSoundEffect(FMOD::Sound* sound)
{
	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	assert(sound);
	if (!sound) { throw std::runtime_error("sound is null"); }

	FMOD_RESULT const r = g_audio->playSound(sound, nullptr, false, nullptr);
	assert(!r);
	if (r)
	{
		throw std::runtime_error("FMOD::System::playSound failed. FMOD_RESULT: " + std::to_string(r));
	}
}


