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
#include <cress/moo/final_act.hpp>

#include "mesh.hpp"
#include "text_renderer.hpp"

void TextRenderer::render()
{
	assert(renderer.program);
	if (!renderer.program) { throw std::runtime_error("program is null"); }

	Mesh*const mesh = dynamic_cast<Mesh*>(renderer.mesh);
	assert(mesh);
	if (!renderer.program) { throw std::runtime_error("text renderer drawable mesh is null or not a mesh"); }

	auto*const material = dynamic_cast<TextMaterial*>(this->renderer.material);
	assert(material);
	if (!material){ throw std::runtime_error("text renderer material is null or not a text material"); }

	glEnable(GL_BLEND);
	CRESS_MOO_FINAL_ACT_SINGLE(fa, glDisable(GL_BLEND));
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

		try {
			renderer.render();
		}
		catch (...) // if failed to render, continue to next character
		{
			unwrittenIndices.push_back(i);
			pos.x += glyph.advance * scale.x;
			continue;
		}

		pos.x += glyph.advance * scale.x;
	}

	// return
	if (!unwrittenIndices.empty())
	{
		throw std::runtime_error("Failed to render all characters of text: " + text);
	}
}
