/*
**	File		:	cress_kia_raw__arrays.hpp
**
**	Summary 	:	Type aliases for raw unchecked bounded array types, with included length.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include "cress_kia__type_conditionals.hpp"
#include "cress_kia_raw__types.hpp"

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia::raw
{
	TM<
		CL T,
		size_t N,
		CL = EnableIf<(N > 0)>
	> using Array = T[N];

	TM<
		CL T,
		size_t N,
		CL = EnableIf<(N > 0)>
	> using ArrayView = UnboundedArrayView<T>;

	TM<
		CL T,
		size_t N,
		CL = EnableIf<(N > 0)>
	> using ArrayOwner = UnboundedArrayOwner<T>;
}
//~ namespace cress::kia::raw



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
