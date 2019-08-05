/*
**	File		:	cress_kia__basic_type_traits.hpp
**
**	Summary 	:	Meta-programming type traits.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include "cress_kia__type_conditionals.hpp"

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia
{
	TM1 using RemoveCV = std::remove_cv_t<T>;

	TM2 CXB isSame = std::is_same_v<T, U>;
	TM2 CXB isSameRemoveCV = isSame<RemoveCV<T>, RemoveCV<U>>;
}
//~ namespace cress::kia



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
