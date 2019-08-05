/*
**	File		:	cress_kia__utils.cpp
**
**	Summary 	:	Miscellaneous utility functions.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	03/04/2019
*/

#pragma once

#include <string_view>

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia
{
	/*
	**	Function	:	charIsAnyOf
	**
	**	Summary 	:	Returns true if the character exists in the string.
	**
	**	Param	c	<char>				:	Character to search for.
	**	Param	str	<std::string_view>	:	String (set of characters) to find it in.
	**	Returns		<bool>				:	True if the character exists in the string.
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	IL CX bool
	charIsAnyOf(char c, std::string_view str) NE
	{
		return str.find(c) != std::string_view::npos;
	}
	//~ fn charIsAnyOf



	/*
	**	Function	:	incrementLooping
	**
	**	Summary 	:	Increments the value, or if it gets too large, loops it to zero.
	**
	**	TParam	T			:	Number type.
	**	Param	val		<T>	:	Value to increment.
	**	Param	length	<T>	:	Maximum limit, exclusive.
	**	Returns			<T>	:	If (val + 1 < length) val + 1, otherwise 0.
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	TM1 IL CX T
	incrementLooping(T val, T length) NE
	{
		return val + 1 < length ? val + 1 : 0;
	}
	//~ fn incrementLooping
}
//~ namespace cress::kia



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
