/*
**	File		:	cress_kia_raw__int_cast.hpp
**
**	Summary 	:	Strongly typed (but unchecked) conversions between raw integral types.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include "cress_kia_raw__int_type_traits.hpp"

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia::raw
{
	TM2IF(isUInt<T> && isSInt<U>)
		ND CX T
	unsignedCast(U x) NE
	{
		return static_cast<T>(x);
	}

	TM2IF(isSInt<T> && isUInt<U>)
		ND CX T
	signedCast(U x) NE
	{
		return static_cast<T>(x);
	}

	TM2IF(isNarrowerInt<T, U>)
		ND CX T
	narrowCast(U x) NE
	{
		return static_cast<T>(x);
	}

	TM2IF(isNarrowerInt<U, T>)
		ND CX T
	wideCast(U x) NE
	{
		return static_cast<T>(x);
	}

	TM2IF(isInt<T> && isInt<U>)
		ND CX T
	intCast(U x) NE
	{
		return static_cast<T>(x);
	}
}
//~ namespace cress::kia::raw



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
