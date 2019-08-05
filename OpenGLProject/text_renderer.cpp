/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	text_renderer.cpp
**
**	Summary		:	Renders text by changing the renderer's mesh quad at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <vector>

#include "mesh.hpp"
#include "text_renderer.hpp"

ReturnCode TextRenderer::render()
{
	ASSERT1(renderer.program);

	Mesh*const mesh = dynamic_cast<Mesh*>(renderer.mesh);
	ASSERT1(mesh);

	auto*const material = dynamic_cast<TextMaterial*>(this->renderer.material);
	ASSERT1(material);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// iterate through the characters of the text
	vec2 pos = position;
	std::vector<size_t> unwrittenIndices;
	for (size_t i = 0; i < text.size(); ++i)
	{
		auto const it = font->characters.find(text[i]);
		if (it == font->characters.end())
		{
			unwrittenIndices.push_back(i);
			continue;
		}

		FontGlyph const& glyph = it->second;

		float const xpos = pos.x + glyph.bearing.x * scale.x;
		float const ypos = pos.y - (glyph.size.y - glyph.bearing.y) * scale.y;
		float const glyphWidth = glyph.size.x * scale.x;
		float const glyphHeight = glyph.size.y * scale.y;

		GLfloat const vertices[] = {
			// position									// uv
			xpos, ypos + glyphHeight, 0, 0,
			xpos, ypos, 0, 1,
			xpos + glyphWidth, ypos, 1, 1,
			xpos + glyphWidth, ypos + glyphHeight, 1, 0,
		};

		// update VBO for this glyph.
		// WARNING: assumes buffer is GL_DYNAMIC_DRAW.
		glBindVertexArray(mesh->vao);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindVertexArray(0);

		// render the glyph over the mesh
		material->tex = glyph.texture;

		if (renderer.render()) // if failed to render, continue to next character
		{
			unwrittenIndices.push_back(i);
			pos.x += glyph.advance * scale.x;
			continue;
		}

		pos.x += glyph.advance * scale.x;
	}

	glDisable(GL_BLEND);

	// return
	if (unwrittenIndices.empty())
	{
		return RC_SUCCESS;
	}
	else
	{
		*g_reason = "Failed to render all characters of text: " + text;
		return RC_PARTIAL;
	}
}
