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

namespace
{
	ReturnCode loadSoundEffects()
	{
		HANDLE_ALL(loadSoundEffect(&g_soundThump, "Thump.wav"));
		HANDLE_ALL(loadSoundEffect(&g_soundZap, "420365__bolkmar__sfx-laser-shot-s.wav"));
		HANDLE_ALL(loadSoundEffect(&g_soundDing, "366104__original-sound__confirmation-downward.wav"));
		// R
		HANDLE_ALL(loadSoundEffect(&g_soundSelect, "Select_SFX.wav"));
		HANDLE_ALL(loadSoundEffect(&g_soundMovePiece, "Solid.wav"));
		HANDLE_ALL(loadSoundEffect(&g_soundNavigate, "bleep.wav"));
		HANDLE_ALL(loadSoundEffect(&g_soundCapture, "sfx_death.ogg"));

		return RC_SUCCESS;
	}

	ReturnCode loadMusicTracks()
	{
		HANDLE_ALL(loadMusicTrack(&g_musicBackground, "Cresspresso - Beyond Afar - 03 Slight Design.wav"));
		// R
		HANDLE_ALL(loadMusicTrack(&g_musicMenuBackground, "COAG - Taboo.mp3"));
		HANDLE_ALL(loadMusicTrack(&g_musicGameBackground, "COAG - The Lost.wav"));

		return RC_SUCCESS;
	}
}



void setReasonFmod(FMOD_RESULT r, std::string const& message)
{
	std::stringstream s;
	s << message << " (FMOD_RESULT " << r << ")";
	*g_reason = s.str();
}



ReturnCode initAudio()
{
	FMOD_RESULT e = FMOD::System_Create(&g_audio);
	if (e)
	{
		g_audio = nullptr;

		setReasonFmod(e, "FMOD::System_Create failed");
		return RC_ERROR;
	}

	e = g_audio->init(numAudioChannels, FMOD_INIT_3D_RIGHTHANDED, nullptr);
	if (e)
	{
		destroyFmodPointer(&g_audio);

		setReasonFmod(e, "FMOD::System::init failed");
		return RC_ERROR;
	}

	bool failed = false;

	if (loadSoundEffects()) { failed = true; }
	if (loadMusicTracks()) { failed = true; }

	return failed ? RC_PARTIAL : RC_SUCCESS;
}



void destroyAudio()
{
	destroyFmodPointer(&g_musicBackground);
	destroyFmodPointer(&g_soundThump);

	destroyFmodPointer(&g_audio);
}



ReturnCode updateAudio()
{
	ASSERT1(g_audio);
	FMOD_RESULT const e = g_audio->update();
	if (e)
	{
		setReasonFmod(e, "FMOD::System::update failed");
		return RC_ERROR;
	}
	return RC_SUCCESS;
}



ReturnCode loadSoundEffect(FMOD::Sound** out, std::string const& fileName)
{
	ASSERT1(g_audio);
	static constexpr char const* const dir = "Resources/Audio/";
	std::string const filePath = dir + fileName;

	FMOD::Sound* p;
	FMOD_RESULT const e = g_audio->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &p);
	if (e)
	{
		setReasonFmod(e, "FMOD::System::createSound: failed to load sound effect at " + filePath);
		return RC_ERROR;
	}

	*out = p;
	return RC_SUCCESS;
}

ReturnCode loadMusicTrack(FMOD::Sound** out, std::string const& fileName)
{
	ASSERT1(g_audio);
	static constexpr char const* const dir = "Resources/Audio/";
	std::string const filePath = dir + fileName;

	FMOD::Sound* p;
	FMOD_RESULT const e = g_audio->createSound(filePath.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &p);
	if (e)
	{
		setReasonFmod(e, "FMOD::System::createSound: failed to load music track at " + filePath);
		return RC_ERROR;
	}

	*out = p;
	return RC_SUCCESS;
}
