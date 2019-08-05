/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	screen.cpp
**
**	Summary		:	GLUT Window information.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "screen.hpp"

namespace screen
{
	static int width = 800;
	static int height = 600;
	static float aspect = (float)width / height;

	int getWidth() noexcept { return width; }
	int getHeight() noexcept { return height; }
	float getAspect() noexcept { return aspect; }
	ivec2 getSize() noexcept { return ivec2(width, height); }
	vec2 getHalfSizeF() noexcept { return 0.5f * vec2(width, height); }

	void update() noexcept
	{
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		aspect = (float)width / height;
	}
}
