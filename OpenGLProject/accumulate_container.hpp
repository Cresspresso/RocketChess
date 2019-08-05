/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	accumulate_container.hpp
**
**	Summary 	:	Returns the accumulated left-fold value across a container.
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/

#pragma once

#include <numeric>

#include "container.hpp"



namespace container
{
	template<class T, class C, class F>
	T accumulate(C const& v, T seed, F op)
	{
		return std::accumulate(container::begin(v), container::end(v), seed, op);
	}

	template<class T, class C>
	T accumulate(C const& v, T seed)
	{
		return std::accumulate(container::begin(v), container::end(v), seed);
	}

	template<class C, class T = typename C::value_type>
	T accumulate(C const& v)
	{
		return accumulate<T>(v, T());
	}



	/*
	**	Function	:	sum
	**
	**	Summary 	:	Returns the sum of all the items in a container.
	**
	**	Project		:	Summative3
	**	Class		:	GD2P01
	**	Author		:	Elijah Shadbolt
	**	Email		:	elijah.sha7979@mediadesign.school.nz
	**	Date Edited	:	22/05/2019
	*/
	template<class C, class T = typename C::value_type>
	T sum(C const& v)
	{
		return accumulate<C, T>(v);
	}
}
