/*
**	File		:	cress_inc__std_streams.hpp
**
**	Summary 	:	Imports input and output stream variables from
**					namespace |std| as static aliases.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

// std library includes
#include <iostream>



// ReSharper disable once CppUnnamedNamespaceInHeaderFile
namespace
{
	// Aliases
	using std::cin;
	using std::cout;
	using std::cerr;
	using std::clog;

	// Renamed Aliases
	/*
	std::istream& g_cin = std::cin;
	std::ostream& g_cout = std::cout;
	std::ostream& g_cerr = std::cerr;
	std::ostream& g_clog = std::clog;
	*/
}
//~ namespace
