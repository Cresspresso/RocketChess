/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	text_renderer.hpp
**
**	Summary		:	Renders text by changing the renderer's mesh quad at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "renderer.hpp"
#include "font.hpp"

// can change modelMatrix via TextRenderer::renderer::modelMatrix property.
struct TextRenderer
{
	Renderer renderer;
	Font* font = nullptr;
	std::string text = "";
	vec2 position = vec2();
	vec2 scale = vec2(1);

	ReturnCode render();
};
