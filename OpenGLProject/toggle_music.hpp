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
void setMusicPaused(bool paused);
void playMusic();
void playMusicG();
void playMusicW();
void stopMusic();
void stopMusicG();
void stopMusicW();
void toggleMusicPaused();
bool isMusicPlaying();

void playSoundEffect(FMOD::Sound* sound);
