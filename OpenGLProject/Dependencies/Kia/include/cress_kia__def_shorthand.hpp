/*
**	File		:	cress_kia__def_shorthand.hpp
**
**	Summary 	:	Defines macros for shorthand constructs.
**					Can be later undefined by |#include "cress_kia__undef_shorthand.hpp"|.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	04/04/2019
*/

//	NOTE:	This file must NOT use #pragma once
//			because it can be later undefined.
#ifndef CRESS_KIA__DEF_SHORTHAND_HPP
#define CRESS_KIA__DEF_SHORTHAND_HPP



#include "cress_kia__macros.hpp"



#pragma push_macro("TM")
#define TM template
#pragma push_macro("TM1")
#define TM1 template<class T>
#pragma push_macro("TM2")
#define TM2 template<class T, class U>
#pragma push_macro("TM1IF")
#define TM1IF(...) template<class T, class = ::std::enable_if_t<(__VA_ARGS__)>>
#pragma push_macro("TM2IF")
#define TM2IF(...) template<class T, class U, class = ::std::enable_if_t<(__VA_ARGS__)>>

#pragma push_macro("TN")
#define TN typename
#pragma push_macro("CL")
#define CL class
#pragma push_macro("CX")
#define CX constexpr
#pragma push_macro("IL")
#define IL inline
#pragma push_macro("OP")
#define OP operator
#pragma push_macro("ND")
#define ND CRESS_KIA__NODISCARD
#pragma push_macro("$")
#define $ const

#pragma push_macro("EX")
#define EX explicit
#pragma push_macro("IM")
#define IM CRESS_KIA__IMPLICIT

#pragma push_macro("NE")
#define NE noexcept
#pragma push_macro("NEF")
#define NEF noexcept(false)
#pragma push_macro("NEIF")
#define NEIF(...) noexcept(__VA_ARGS__)
#pragma push_macro("NEX")
#define NEX(...) noexcept(noexcept(__VA_ARGS__))

#pragma push_macro("CXB")
#define CXB inline constexpr bool const



#endif //~ ifndef CRESS_KIA__DEF_SHORTHAND_HPP
