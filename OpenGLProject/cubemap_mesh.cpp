/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cubemap_mesh.cpp
**
**	Summary		:	Inside-facing cube, used for rendering skyboxes and cubemaps.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "cubemap_mesh.hpp"

namespace
{
	static constexpr float const x = 0.5f;
}

GLfloat const vertices[] = {
	// Front
	-x,  x,  x,
	-x, -x,  x,
	 x, -x,  x,
	 x,  x,  x,
	// Back
	-x,  x, -x,
	-x, -x, -x,
	 x, -x, -x,
	 x,  x, -x,
	// Left
	-x,  x, -x,
	-x, -x, -x,
	-x, -x,  x,
	-x,  x,  x,
	// Right
	 x,  x,  x,
	 x, -x,  x,
	 x, -x, -x,
	 x,  x, -x,
	// Top
	-x,  x, -x,
	-x,  x,  x,
	 x,  x,  x,
	 x,  x, -x,
	// Bottom
	-x, -x,  x,
	-x, -x, -x,
	 x, -x, -x,
	 x, -x,  x,
};

GLuint const indices[] = {
	// Front
	0, 2, 1,
	0, 3, 2,
	// Back
	7, 5, 6,
	7, 4, 5,
	// Left
	8, 10, 9,
	8, 11, 10,
	// Right
	12, 14, 13,
	12, 15, 14,
	// Top
	16, 18, 17,
	16, 19, 18,
	// Bottom
	20, 22, 21,
	20, 23, 22,
};

Mesh makeCubeMapMesh()
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	GLsizei const vertexSize = 3 * sizeof(GLfloat);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	glEnableVertexAttribArray(0);

	Mesh mesh;
	mesh.vao = a;
	mesh.vbo = b;
	mesh.ebo = e;
	mesh.indicesCount = sizeof(indices) / sizeof(indices[0]);
	mesh.drawMode = GL_TRIANGLES;
	return mesh;
}
