/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	audio.cpp
**
**	Summary		:	Audio system and sound resources.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <sstream>

#include "exceptions.hpp"

#include "audio.hpp"



FMOD::System* g_audio = nullptr;

FMOD::Sound* g_soundThump = nullptr;
FMOD::Sound* g_soundZap = nullptr;
FMOD::Sound* g_soundDing = nullptr;
// R
FMOD::Sound* g_soundSelect = nullptr;
FMOD::Sound* g_soundMovePiece = nullptr;
FMOD::Sound* g_soundNavigate = nullptr;
FMOD::Sound* g_soundCapture = nullptr;


FMOD::Sound* g_musicBackground = nullptr;
// R
FMOD::Sound* g_musicMenuBackground = nullptr;
FMOD::Sound* g_musicGameBackground = nullptr;
FMOD::Sound* g_musicWinBackground = nullptr;

namespace
{
	void loadSoundEffects()
	{
		try { g_soundThump = loadSoundEffect("Thump.wav"); }
		catch (...) { printException(); }

		try { g_soundZap=loadSoundEffect("420365__bolkmar__sfx-laser-shot-s.wav"); }
		catch (...) { printException(); }

		try { g_soundDing=loadSoundEffect( "366104__original-sound__confirmation-downward.wav"); }
		catch (...) { printException(); }
		// R
		try { g_soundSelect=loadSoundEffect( "Select_SFX.wav"); }
		catch (...) { printException(); }

		try { g_soundMovePiece=loadSoundEffect( "Solid.wav"); }
		catch (...) { printException(); }

		try { g_soundNavigate=loadSoundEffect( "bleep.wav"); }
		catch (...) { printException(); }

		try { g_soundCapture= loadSoundEffect( "sfx_death.ogg"); }
		catch (...) { printException(); }
	}

	void loadMusicTracks()
	{
		// R
		try { g_musicMenuBackground=loadMusicTrack( "COAG - Taboo.mp3"); }
		catch (...) { printException(); }

		try { g_musicGameBackground=loadMusicTrack( "COAG - The Lost.wav"); }
		catch (...) { printException(); }

		try { g_musicWinBackground = loadMusicTrack("Background Music - If I Don't Return CO.AG.mp3"); }
		catch (...) { printException(); }
	}
}



void initAudio()
{
	FMOD::System* audio;
	FMOD_RESULT e = FMOD::System_Create(&audio);
	if (e)
	{
		audio = nullptr;
		throw std::runtime_error("FMOD::System_Create failed.  FMOD_RESULT: " + std::to_string(e));
	}

	e = audio->init(numAudioChannels, FMOD_INIT_3D_RIGHTHANDED, nullptr);
	if (e)
	{
		destroyFmodPointer(audio);
		throw std::runtime_error("FMOD::System::init failed.  FMOD_RESULT: " + std::to_string(e));
	}

	g_audio = audio;

	bool failed = false;

	try { loadSoundEffects(); }
	catch (...) { printException(); }

	try { loadMusicTracks(); }
	catch (...) { printException(); }

	if (failed) { throw std::runtime_error("Some audio files failed to load."); }
}



void destroyAudio()
{
	destroyFmodPointer(g_audio);
}



void updateAudio()
{
	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	FMOD_RESULT const e = g_audio->update();
	if (e)
	{
		throw std::runtime_error("FMOD::System::update failed. FMOD_RESULT: " + std::to_string(e));
	}
}



FMOD::Sound* loadSoundEffect(std::string const& fileName)
{
	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	static constexpr char const* const dir = "Resources/Audio/";
	std::string const filePath = dir + fileName;

	FMOD::Sound* p;
	FMOD_RESULT const e = g_audio->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &p);
	if (e)
	{
		throw std::runtime_error("FMOD::System::createSound: failed to load sound effect at " + filePath + "\nFMOD_RESULT: " + std::to_string(e));
	}

	return p;
}

FMOD::Sound* loadMusicTrack(std::string const& fileName)
{
	assert(g_audio);
	if (!g_audio) { throw std::runtime_error("audio is null"); }

	static constexpr char const* const dir = "Resources/Audio/";
	std::string const filePath = dir + fileName;

	FMOD::Sound* p;
	FMOD_RESULT const e = g_audio->createSound(filePath.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &p);
	if (e)
	{
		throw std::runtime_error("FMOD::System::createSound: failed to load music track at " + filePath + "\nFMOD_RESULT: " + std::to_string(e));
	}

	return p;
}
