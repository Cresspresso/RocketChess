/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	statistic_container.hpp
**
**	Summary 	:	Statistical operations on containers.
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/

#pragma once

#include "container.hpp"
#include "accumulate_container.hpp"



namespace container
{
	/*
	**	Returns iterator to item in the exact middle of the container.
	*/
	template<class C>
	auto medianIt(C const& v)
	{
		auto sz = container::size(v);
		auto it = container::begin(v);
		std::advance(it, sz / 2);
		return it;
	}



	/*
	**	Returns mean value of container.
	*/
	template<class C, class Divisor = float>
	auto mean(C const& v)
	{
		Divisor const sz = static_cast<Divisor>(container::size(v));
		return container::sum(v) / sz;
	}
}
