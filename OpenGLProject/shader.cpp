/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	shader.cpp
**
**	Summary		:	Loads OpenGL Shaders.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#include <vector>
#include <sstream>

#include <cress/moo/final_act.hpp>

#include "shader.hpp"



GLuint makeShader(GLenum type, char const* const sourceCode)
{
	GLuint v = glCreateShader(type);
	glShaderSource(v, 1, &sourceCode, nullptr);
	glCompileShader(v);

	GLint success = 0;
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		CRESS_MOO_FINAL_ACT_SINGLE(fa, glDeleteShader(v));

		GLint maxLength;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, errorLog.data());
		throw std::runtime_error(std::string("Error compiling shader: ") + errorLog.data());
	}
	return v;
}

GLuint makeProgram(GLuint v, GLuint f)
{
	GLuint p = glCreateProgram();

	glAttachShader(p, v);
	glAttachShader(p, f);

	CRESS_MOO_FINAL_ACT_BEGIN(fa);
	glDetachShader(p, f);
	glDetachShader(p, v);
	CRESS_MOO_FINAL_ACT_END(fa);

	glLinkProgram(p);

	GLint success = 0;
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success)
	{
		CRESS_MOO_FINAL_ACT_BEGIN(fa);
		glDeleteProgram(p);
		CRESS_MOO_FINAL_ACT_END(fa);

		GLint maxLength;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(p, maxLength, &maxLength, errorLog.data());
		throw std::runtime_error(std::string("Error linking program: ") + errorLog.data());
	}

	return p;
}

GLuint loadProgram(GLuint* v, GLuint* f, std::string const& name)
{
	static char const* const dir = "Resources/Shaders/";
	std::string vsrc = readEntireFile(dir + name + ".verts");
	std::string fsrc = readEntireFile(dir + name + ".frags");

	bool failure = true;

	*v = makeShader(GL_VERTEX_SHADER, vsrc.data());
	CRESS_MOO_FINAL_ACT_BEGIN(fav);
	if (failure) { glDeleteShader(*v); }
	CRESS_MOO_FINAL_ACT_END(fav);

	*f = makeShader(GL_FRAGMENT_SHADER, fsrc.data());
	CRESS_MOO_FINAL_ACT_BEGIN(faf);
	if (failure) { glDeleteShader(*f); }
	CRESS_MOO_FINAL_ACT_END(faf);

	GLuint p = makeProgram(*v, *f);

	failure = false;
	return p;
}
