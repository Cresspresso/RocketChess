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
#include "read_entire_file.hpp"

GLuint makeShader(GLenum type, char const* const sourceCode);
GLuint makeProgram(GLuint v, GLuint f);

// takes file name without directory or file extension
GLuint loadProgram(GLuint* v, GLuint* f, std::string const& name);
