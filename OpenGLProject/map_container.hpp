/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	map_container.hpp
**
**	Summary 	:	Creates a container by selecting items derived from each item in a container.
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/

#pragma once

#include <vector>



namespace container
{
	/*
	**	Function	:	mapAs
	**
	**	Summary 	:	Creates a container by selecting items derived from each item in a container.
	**
	**	TParam		:	T
	**					The value_type of items in the returned container.
	**
	**	Param		:	v
	**					The container to loop over.
	**
	**	Param		:	selector
	**					Functor that returns a result item based on an item from the input container.
	**
	**	Returns		:	Container (same size as input container) of elements returned by the selector.
	**
	**	Project		:	Summative3
	**	Class		:	GD2P01
	**	Author		:	Elijah Shadbolt
	**	Email		:	elijah.sha7979@mediadesign.school.nz
	**	Date Edited	:	22/05/2019
	*/
	template<class T, class C, class P>
	std::vector<T>
		mapAs(C const& v, P selector)
	{
		std::vector<T> r;
		for (auto const& x : v)
		{
			r.push_back(selector(x));
		}
		return r;
	}



	/*
	**	Function	:	map
	**
	**	Summary 	:	Creates a container by selecting items derived from each item in a container.
	**
	**	Param		:	v
	**					The container to loop over.
	**
	**	Param		:	selector
	**					Functor that returns a result item based on an item from the input container.
	**
	**	Returns		:	Container (same size as input container) of elements returned by the selector.
	**
	**	Project		:	Summative3
	**	Class		:	GD2P01
	**	Author		:	Elijah Shadbolt
	**	Email		:	elijah.sha7979@mediadesign.school.nz
	**	Date Edited	:	22/05/2019
	*/
	template<class C, class P>
	std::vector<std::invoke_result_t<P, typename C::value_type>>
		map(C const& v, P selector)
	{
		return mapAs<std::invoke_result_t<P, typename C::value_type>>(v, selector);
	}
}
