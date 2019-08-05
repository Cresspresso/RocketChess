/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	pyramid.cpp
**
**	Summary		:	Pyramid mesh.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "pyramid.hpp"

GLfloat const vertices[] = {
	-0.5f, 0, -0.5f,	0, -1, 0,	0, 0,
	-0.5f, 0, 0.5f,		0, -1, 0,	0, 1,
	0.5f, 0, 0.5f,		0, -1, 0,	1, 1,
	0.5f, 0, -0.5f,		0, -1, 0,	1, 0,

	0, 0.5f, 0,			0, 1, 0,	0.5f, 0.5f,
};

GLuint const indices[] = {
	0, 4, 3,
	3, 4, 2,
	2, 4, 1,
	1, 4, 0,

	1, 0, 3,
	1, 3, 2,
};

Mesh makePyramidMesh()
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


	GLsizei const vertexSize = 8 * sizeof(GLfloat);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	Mesh mesh;
	mesh.vao = a;
	mesh.vbo = b;
	mesh.ebo = e;
	mesh.indicesCount = sizeof(indices) / sizeof(indices[0]);
	mesh.drawMode = GL_TRIANGLES;
	return mesh;
}
