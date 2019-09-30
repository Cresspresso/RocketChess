/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	toggle_music.hpp
**
**	Summary		:	Functions for playing, pausing, and stopping the background music.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

bool isMusicPaused();
ReturnCode setMusicPaused(bool paused);
ReturnCode playMusic();
ReturnCode playMusicG();
ReturnCode stopMusic();
ReturnCode stopMusicG();
ReturnCode toggleMusicPaused();
bool isMusicPlaying();

ReturnCode playSoundEffect(FMOD::Sound* sound);
