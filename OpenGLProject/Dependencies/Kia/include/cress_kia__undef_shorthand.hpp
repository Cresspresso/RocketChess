/*
**	File		:	cress_kia__undef_shorthand.hpp
**
**	Summary 	:	Undefines shorthand macros.
**					Can be later defined by |#include "cress_kia__def_shorthand.hpp"|.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	04/04/2019
*/

//	NOTE:	This file must NOT use regular include guards or #pragma once
//			because it can later be defined again.
// ReSharper disable once CppMissingIncludeGuard
#ifndef CRESS_KIA__DEF_SHORTHAND_HPP
#error "cress::kia - Shorthand macros are already undefined."
#else
#undef CRESS_KIA__DEF_SHORTHAND_HPP



#pragma pop_macro("TM")
#pragma pop_macro("TM1")
#pragma pop_macro("TM2")
#pragma pop_macro("TM1IF")
#pragma pop_macro("TM2IF")

#pragma pop_macro("TN")
#pragma pop_macro("CL")
#pragma pop_macro("CX")
#pragma pop_macro("IL")
#pragma pop_macro("OP")
#pragma pop_macro("ND")
#pragma pop_macro("$")

#pragma pop_macro("EX")
#pragma pop_macro("IM")

#pragma pop_macro("NE")
#pragma pop_macro("NEF")
#pragma pop_macro("NEIF")
#pragma pop_macro("NEX")

#pragma pop_macro("CXB")



#endif //~ ifndef CRESS_KIA__DEF_SHORTHAND_HPP
