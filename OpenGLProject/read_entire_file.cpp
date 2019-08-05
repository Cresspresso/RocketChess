/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	read_entire_file.cpp
**
**	Summary		:	Reads an entire file and stores it in memory as one string.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <fstream>

#include "read_entire_file.hpp"



std::string readEntireStream(std::istream& ins)
{
	std::string content;

	// Determine the size of of the file in characters and resize the string variable to accomodate
	ins.seekg(0, std::ios::end);
	content.resize(static_cast<size_t>(ins.tellg()));

	// Set the position of the next character to be read back to the beginning
	ins.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	ins.read(&content[0], content.size());

	return content;
}
//~ readEntireStream



std::string readEntireFile(std::string const& filePath)
{
	// Open the file for reading
	std::ifstream file{ filePath, std::ios::in };

	// Ensure the file is open and readable
	if (!file.good())
	{
		throw std::runtime_error{ "Error reading file: " + filePath };
	}

	return readEntireStream(file);
}
//~ readEntireFile
