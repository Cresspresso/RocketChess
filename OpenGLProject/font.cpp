/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	font.cpp
**
**	Summary		:	Loads FreeType Fonts.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#include <map>
#include <vector>
#include <sstream>

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <soil/SOIL.h>

#include <ft/ft2build.h>
#include FT_FREETYPE_H

#include "globals.hpp"

#include "font.hpp"

void FontGlyph::destroy() noexcept
{
	if (texture) { glDeleteTextures(1, &texture); texture = 0; }
}

void Font::destroy() noexcept
{
	for (auto [key, glyph] : characters)
	{
		glyph.destroy();
	}
}

namespace
{
	static GLuint makeTexture(FT_Face face)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		auto const& b = face->glyph->bitmap;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, b.width, b.rows, 0, GL_RED, GL_UNSIGNED_BYTE, b.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return texture;
	}
}

Font loadFont(
	std::string const& fileName,
	unsigned int glyphWidth,
	unsigned int glyphHeight
)
{
	Font font;

	static constexpr char const* const dir = "Resources/Fonts/";
	std::string const filePath = dir + fileName + ".ttf";

	FT_Library ft;
	FT_Error e = FT_Init_FreeType(&ft);
	if (e)
	{
		throw std::runtime_error("FREETYPE: Failed to init FreeType library. FT_Error: " + std::to_string(e));
	}

	FT_Face face;
	e = FT_New_Face(ft, filePath.c_str(), 0, &face);
	if (e)
	{
		throw std::runtime_error("FREETYPE: Failed to load font at: " + filePath + "   FT_Error: " + std::to_string(e));
	}

	e = FT_Set_Pixel_Sizes(face, glyphWidth, glyphHeight);
	if (e)
	{
		throw std::runtime_error("FREETYPE: Failed set pixel sizes. FT_Error: " + std::to_string(e));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load the glyphs
	std::map<GLchar, FontGlyph> characters;

	std::vector<GLubyte> notLoaded;
	for (GLubyte c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			notLoaded.push_back(c);
			continue;
		}

		GLuint texture = makeTexture(face);

		auto const* p = face->glyph;
		FontGlyph glyph;
		glyph.texture = texture;
		glyph.size = ivec2(p->bitmap.width, p->bitmap.rows);
		glyph.bearing = ivec2(p->bitmap_left, p->bitmap_top);
		glyph.advance = p->advance.x >> 6;
		
		characters.insert(std::pair<GLchar, FontGlyph>(c, glyph));
	}

	// cleanup
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// apply to out parameter
	font.characters = std::move(characters);

	// return
	if (!notLoaded.empty())
	{
		std::stringstream s;
		s << "Failed to Load some Glyphs for characters: ";
		for (auto const c : notLoaded)
		{
			s << c << ", ";
		}
		throw std::runtime_error(s.str());
	}

	return font;
}



GLuint const indices[] = {
	0, 1, 2,
	2, 3, 0,
};

Mesh makeFontMesh()
{
	GLuint a;
	glGenVertexArrays(1, &a);
	glBindVertexArray(a);

	GLuint e;
	glGenBuffers(1, &e);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint b;
	glGenBuffers(1, &b);
	glBindBuffer(GL_ARRAY_BUFFER, b);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);


	GLsizei const vertexSize = 4 * sizeof(GLfloat);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	Mesh mesh;
	mesh.vao = a;
	mesh.vbo = b;
	mesh.ebo = e;
	mesh.indicesCount = sizeof(indices) / sizeof(indices[0]);
	mesh.drawMode = GL_TRIANGLES;
	return mesh;
}



void TextMaterial::apply(GLuint program)
{
	Super::apply(program);

	glUniform3f(glGetUniformLocation(program, "tint"), tint.r, tint.g, tint.b);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	glDisable(GL_CULL_FACE);
}

void TextMaterial::unapply(GLuint program)
{
	Super::unapply(program);

	glEnable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
