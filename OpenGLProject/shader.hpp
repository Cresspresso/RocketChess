/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	shader.hpp
**
**	Summary		:	Loads OpenGL Shaders.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#pragma once
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include "errors.hpp"
#include "read_entire_file.hpp"
ReturnCode makeShader(GLuint* out, GLenum type, char const* const sourceCode);
ReturnCode makeProgram(GLuint* out, GLuint v, GLuint f);

// takes file name without directory or file extension
ReturnCode loadProgram(GLuint* p, GLuint* v, GLuint* f, std::string const& name);
