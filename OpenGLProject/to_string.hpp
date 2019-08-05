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
#include <string_view>
#include <array>
//#include <vector>

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



#ifdef COMMENT

template<size_t N>
std::string_view toStringView(std::array<char, N> const& buffer)
{
	return std::string_view{ buffer.data(), buffer.size() };
}

template<size_t N>
std::string to_string(std::array<char, N> const& buffer)
{
	return std::string{ buffer.data(), buffer.size() };
}



namespace string_utils
{
	template<class C>
	struct HasContiguousMemory : std::false_type {};

	template<class T>
	struct HasContiguousMemory<T[]> : std::true_type {};

	template<class T, size_t N>
	struct HasContiguousMemory<T[N]> : std::true_type {};

	template<class T, size_t N>
	struct HasContiguousMemory<std::array<T, N>> : std::true_type {};

	template<class T>
	struct HasContiguousMemory<std::vector<T>> : std::true_type {};



	template<class T>
	std::string to_string(T const& container)
	{
		using std::to_string;
		using string_utils::to_string;

		using std::begin;
		using std::end;

		std::ostringstream s;
		s << '{';
		auto const e = end(container);
		auto it = begin(container);
		if (it != e)
		{
			s << to_string(*it);
			for (++it; it != e; ++it)
			{
				s << ", " << to_string(*it);
			}
		}
		s << '}' << std::ends;
		return s.str();
	}
}
#endif //~ COMMENT
