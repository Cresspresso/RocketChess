/*
**	File		:	cress_kia_raw__int_type_traits.hpp
**
**	Summary 	:	Meta-programming traits for raw integer types.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	03/04/2019
*/

#pragma once

#include "cress_kia__basic_type_traits.hpp"
#include "cress_kia_raw__types.hpp"

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia::raw
{
	TM1 struct TIsUInt : public BoolConditional<
		isSameRemoveCV<T, UByte> ||
		isSameRemoveCV<T, UShort> ||
		isSameRemoveCV<T, UInt> ||
		isSameRemoveCV<T, ULong> ||
		isSameRemoveCV<T, ULLong>
	> {};
	TM1 CXB isUInt = TIsUInt<T>::value;



	TM1 struct TIsSInt : public BoolConditional<
		isSameRemoveCV<T, SByte> ||
		isSameRemoveCV<T, Short> ||
		isSameRemoveCV<T, Int> ||
		isSameRemoveCV<T, Long> ||
		isSameRemoveCV<T, LLong>
	> {};
	TM1 CXB isSInt = TIsSInt<T>::value;



	TM1 struct TIsInt : public BoolConditional<isUInt<T> || isSInt<T>> {};
	TM1 CXB isInt = TIsInt<T>::value;



	TM2IF(isUInt<T> && isUInt<U>)
	struct TIsNarrowerUInt : public BoolConditional<
		(isSameRemoveCV<T, UByte> && (
			isSameRemoveCV<U, UShort> ||
			isSameRemoveCV<U, UInt> ||
			isSameRemoveCV<U, ULong> ||
			isSameRemoveCV<U, ULLong>))
		|| (isSameRemoveCV<T, UShort> && (
			isSameRemoveCV<U, UInt> ||
			isSameRemoveCV<U, ULong> ||
			isSameRemoveCV<U, ULLong>))
		|| (isSameRemoveCV<T, UInt> && (
			isSameRemoveCV<U, ULong> ||
			isSameRemoveCV<U, ULLong>))
		|| (isSameRemoveCV<T, ULong> && (
			isSameRemoveCV<U, ULLong>))
		> {};
	TM2 CXB isNarrowerUInt = TIsNarrowerUInt<T, U>::value;



	TM2IF(isSInt<T> && isSInt<U>)
	struct TIsNarrowerSInt : public BoolConditional<
		(isSameRemoveCV<T, SByte> && (
			isSameRemoveCV<U, Short> ||
			isSameRemoveCV<U, Int> ||
			isSameRemoveCV<U, Long> ||
			isSameRemoveCV<U, LLong>))
		|| (isSameRemoveCV<T, Short> && (
			isSameRemoveCV<U, Int> ||
			isSameRemoveCV<U, Long> ||
			isSameRemoveCV<U, LLong>))
		|| (isSameRemoveCV<T, Int> && (
			isSameRemoveCV<U, Long> ||
			isSameRemoveCV<U, LLong>))
		|| (isSameRemoveCV<T, Long> && (
			isSameRemoveCV<U, LLong>))
		> {};
	TM2 CXB isNarrowerSInt = TIsNarrowerSInt<T, U>::value;


	TM2IF(isInt<T> && isInt<U>)
	struct TIsSameSign : public BoolConditional<
		(isSInt<T> && isSInt<U>) ||
		(isUInt<T> && isUInt<U>)
		> {};
	TM2 CXB isSameSign = TIsSameSign<T, U>::value;



	TM<	CL T,
		CL U,
		bool isSignedTag = true,
		CL = EnableIf<(isSameSign<T, U>)>>
	struct TIsNarrowerInt : public BoolConditional<
		isNarrowerSInt<T, U>
		>
	{};
	TM2 struct TIsNarrowerInt<T, U, false> : public BoolConditional<
		isNarrowerUInt<T, U>
		>
	{};
	TM2 CXB isNarrowerInt = TIsNarrowerInt<T, U, isSInt<T>>::value;
}
//~ namespace cress::kia::raw



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
