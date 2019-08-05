/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	quad.cpp
**
**	Summary		:	Sphere mesh.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <glm/gtc/constants.hpp>

#include "sphere.hpp"



Mesh makeSphereMesh()
{
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	float phi = 0;
	float theta = 0;

	float vertices[(sections) * (sections)* vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (glm::pi<float>() / (sections - 1));
		}

		phi += (2 * glm::pi<float>()) / (sections - 1);
	}


	GLuint indices[(sections) * (sections)* indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
		}
	}

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
