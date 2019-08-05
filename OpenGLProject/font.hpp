/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	font.hpp
**
**	Summary		:	Loads FreeType Fonts.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#pragma once

#include <map>
#include <string>

#include "common.hpp"
#include "mesh.hpp"
#include "mvp_material.hpp"
#include "renderer.hpp"



struct FontGlyph
{
	GLuint texture = 0;
	ivec2 size = {};
	ivec2 bearing = {};
	int advance = 0;

	void destroy() noexcept;
};

struct Font
{
	std::map<GLchar, FontGlyph> characters;

	void destroy() noexcept;
};

ReturnCode loadFont(
	Font* font,
	std::string const& fileName,
	unsigned int glyphWidth = 48,
	unsigned int glyphHeight = 0
);
//ReturnCode loadFont(
//	std::unique_ptr<Font>& font,
//	std::string const& fileName,
//	unsigned int glyphWidth = 48,
//	unsigned int glyphHeight = 0
//);



Mesh makeFontMesh();



struct TextMaterial : public MvpMaterial
{
	using Super = MvpMaterial;
	~TextMaterial() = default;

	ReturnCode apply(GLuint program) override;
	ReturnCode unapply(GLuint program) override;



	vec3 tint = vec3(1);
	GLuint tex = 0;
};
