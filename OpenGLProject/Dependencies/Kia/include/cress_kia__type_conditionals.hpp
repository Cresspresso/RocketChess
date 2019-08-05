/*
**	File		:	cress_kia__type_conditionals.hpp
**
**	Summary 	:	Basic meta-programming conditional types.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include <type_traits>

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia
{
	TM<CL T, T V>
	using IntegralConstant = std::integral_constant<T, V>;
	TM<bool B>
	using BoolConstant = std::bool_constant<B>;

	using TrueType = std::true_type;
	using FalseType = std::false_type;

	TM<bool B, CL T, CL F>
	using Conditional = std::conditional_t<B, T, F>;

	TM<bool B>
	using BoolConditional = Conditional<B, TrueType, FalseType>;

	TM<bool B, CL T = void>
	using EnableIf = std::enable_if_t<B, T>;
}
//~ namespace cress::kia



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
