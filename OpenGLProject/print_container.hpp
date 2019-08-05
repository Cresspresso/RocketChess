/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	print_container.hpp
**
**	Summary		:	Functions for printing containers as text.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "container.hpp"

namespace container
{
	template<class C, class P>
	std::ostream& printAs(
		std::ostream& outs,
		C const& v,
		P itemPrinter,
		char const* const prefix = "{",
		char const* const suffix = "}",
		char const* const separator = ", "
	)
	{
		outs << prefix;
		auto it = container::begin(v);
		auto const e = container::end(v);
		if (it != e)
		{
			itemPrinter(outs, *it);
			for (++it; it != e; ++it)
			{
				outs << separator;
				itemPrinter(outs, *it);
			}
		}
		outs << suffix;
		return outs;
	}



	/*
	**	Same as printAs() but using operator<<(std::ostream&, value_type const&)
	*/
	template<class C>
	std::ostream& print(
		std::ostream& outs,
		C const& v,
		char const* const prefix = "{",
		char const* const suffix = "}",
		char const* const separator = ", "
	)
	{
		return printAs(
			outs, v,
			[](std::ostream& outs, auto const& item) -> std::ostream& { return outs << item; },
			prefix, suffix, separator
		);
	}



	template<class C>
	std::string toString(C const& v)
	{
		std::ostringstream s;
		print(s, v);
		return s.str();
	}
}
