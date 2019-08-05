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

#include "shader.hpp"



ReturnCode makeShader(GLuint* out, GLenum type, char const* const sourceCode)
{
	GLuint v = glCreateShader(type);
	glShaderSource(v, 1, &sourceCode, nullptr);
	glCompileShader(v);

	GLint success = 0;
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, errorLog.data());
		*g_reason = std::string("Error compiling shader: ") + errorLog.data();

		errorLog.clear();
		glDeleteShader(v);
		//v = 0;
		return RC_ERROR;
	}
	*out = v;
	return RC_SUCCESS;
}

ReturnCode makeProgram(GLuint* out, GLuint v, GLuint f)
{
	GLuint p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	GLint success = 0;
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint maxLength;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(p, maxLength, &maxLength, errorLog.data());
		*g_reason = std::string("Error linking program: ") + errorLog.data();

		errorLog.clear();
		glDetachShader(p, f);
		glDetachShader(p, v);
		glDeleteProgram(p);
		//p = 0;
		return RC_ERROR;
	}
	glDetachShader(p, f);
	glDetachShader(p, v);
	*out = p;
	return RC_SUCCESS;
}

ReturnCode loadProgram(GLuint* p, GLuint* v, GLuint* f, std::string const& name)
{
	try
	{
		static char const* const dir = "Resources/Shaders/";
		std::string vsrc = readEntireFile(dir + name + ".verts");
		std::string fsrc = readEntireFile(dir + name + ".frags");
		if (makeShader(v, GL_VERTEX_SHADER, vsrc.data()))
		{
			return RC_ERROR;
		}
		if (makeShader(f, GL_FRAGMENT_SHADER, fsrc.data()))
		{
			glDeleteShader(*v);
			v = 0;
			return RC_ERROR;
		}
		if (makeProgram(p, *v, *f))
		{
			glDeleteShader(*v);
			v = 0;
			glDeleteShader(*f);
			f = 0;
			return RC_ERROR;
		}
		return RC_SUCCESS;
	}
	CATCH_RE();
}
