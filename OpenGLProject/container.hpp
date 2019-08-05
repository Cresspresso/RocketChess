/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	container.hpp
**
**	Summary		:	Wrapper functions for std library container functions.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	13/06/2019
*/

#pragma once

namespace container
{
	template<class C>
	auto begin(C& v)
	{
		using std::begin;
		return begin(v);
	}

	template<class C>
	auto end(C& v)
	{
		using std::end;
		return end(v);
	}



	template<class C>
	auto cbegin(C& v)
	{
		using std::cbegin;
		return cbegin(v);
	}

	template<class C>
	auto cend(C& v)
	{
		using std::cend;
		return cend(v);
	}



	template<class C>
	auto size(C& v)
	{
		using std::size;
		return size(v);
	}

	template<class T, size_t N>
	constexpr size_t size(T(&v)[N])
	{
		return N;
	}
}
