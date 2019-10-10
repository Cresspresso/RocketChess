/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mesh.hpp
**
**	Summary		:	Wrapper for OpenGL buffer objects.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "drawable.hpp"

class Mesh : public IDrawable
{
public:
	GLuint vao = 0; // vertex array object
	GLuint vbo = 0; // vertex buffer object
	GLuint ebo = 0; // element buffer object
	GLuint indicesCount = 0;
	GLenum drawMode = GL_TRIANGLES;

	void draw() override;
	void destroy() noexcept;


	/*Mesh() = default;
	Mesh(Mesh const& that) = delete;
	Mesh& operator=(Mesh const& that) = delete;

	virtual ~Mesh() noexcept;
	Mesh(Mesh&& that) noexcept;
	Mesh& operator=(Mesh&& that) noexcept;
	void swap(Mesh& that) noexcept;
	friend void swap(Mesh& a, Mesh& b) { a.swap(b); }*/
};
