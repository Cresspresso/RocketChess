/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	read_entire_file.hpp
**
**	Summary		:	Reads an entire file and stores it in memory as one string.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <string>
#include <iosfwd>



std::string readEntireStream(std::istream& ins);
std::string readEntireFile(std::string const& filePath);
