/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/moo/lambda.hpp
//
//	Summary		:	Shorthand lambda macros.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	29/09/2019
*/

#pragma once



#define LAM0(expression)	([&]()						noexcept(noexcept(expression))				{ return (expression); })
#define LAM0R(expression)	([&]()						noexcept(noexcept(expression))	->auto&&	{ return (expression); })
#define LAM1(expression)	([&](auto&& _0)				noexcept(noexcept(expression))				{ return (expression); })
#define LAM1R(expression)	([&](auto&& _0)				noexcept(noexcept(expression))	->auto&&	{ return (expression); })
#define LAM2(expression)	([&](auto&& _0, auto&& _1)	noexcept(noexcept(expression))				{ return (expression); })
#define LAM2R(expression)	([&](auto&& _0, auto&& _1)	noexcept(noexcept(expression))	->auto&&	{ return (expression); })
