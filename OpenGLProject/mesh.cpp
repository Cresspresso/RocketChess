/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mesh.cpp
**
**	Summary		:	Wrapper for OpenGL buffer objects.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "mesh.hpp"

void Mesh::draw()
{
	assert(vao);
	if (!vao) { throw std::runtime_error("vao is null"); }
	assert(vbo);
	if (!vbo) { throw std::runtime_error("vbo is null"); }
	assert(ebo);
	if (!ebo) { throw std::runtime_error("ebo is null"); }
	assert(indicesCount);
	if (!indicesCount) { throw std::runtime_error("indicesCount is zero"); }

	glBindVertexArray(vao);
	glDrawElements(drawMode, indicesCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::destroy() noexcept
{
	if (vao) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (vbo) { glDeleteBuffers(1, &vbo); vbo = 0; }
	if (ebo) { glDeleteBuffers(1, &ebo); ebo = 0; }
	indicesCount = 0;
}

//Mesh::~Mesh() noexcept
//{
//	destroy();
//}
//
//Mesh::Mesh(Mesh&& that) noexcept
//{
//	swap(that);
//}
//
//Mesh& Mesh::operator=(Mesh&& that) noexcept
//{
//	swap(that);
//}
//
//void Mesh::swap(Mesh& that)
//{
//	using std::swap;
//	swap(vao, that.vao);
//	swap(vbo, that.vbo);
//	swap(ebo, that.ebo);
//	swap(indicesCount, that.indicesCount);
//	swap(drawMode, that.drawMode);
//}
