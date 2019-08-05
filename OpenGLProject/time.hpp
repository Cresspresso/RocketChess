/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	time.hpp
**
**	Summary		:	Keeps track of delta time.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

namespace Time
{
	float getDeltaTime() noexcept;
	float getCurrentTime() noexcept;
	void update() noexcept;
	void init() noexcept;
}
