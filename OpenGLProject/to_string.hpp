/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	to_string.hpp
**
**	Summary		:	Utility functions for converting objects to strings.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	18/06/2019
*/

#pragma once

#include <string>
#include <sstream>

#include "compatibility.hpp"



template<class T>
std::string toString(T const& value)
{
	using std::to_string;
	return to_string(value);
}



template<class Arg>
std::ostream& printPack(std::ostream& outs, Arg&& arg)
{
	return outs << std::forward<Arg>(arg);
}

template<class Arg, class...Args>
std::ostream& printPack(std::ostream& outs, Arg&& arg, Args&&...args)
{
	printPack(outs, arg);
	return printPack(outs, args...);
}



template<class...Args>
std::string stringLink(Args&& ...args)
{
	std::ostringstream s;
#ifdef OldMsvc
	printPack(s, std::forward<Args>(args)...);
#else
	(s << ... << args);
#endif
	return s.str();
}


