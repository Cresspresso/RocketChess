/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	lambda.hpp
**
**	Summary		:	Shorthand macros for lambda expressions.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once



/* Shorthand lambda expression with function body. */
#define LAM_EX(argument, body)\
	[&](auto&& argument){body}

/* Shorthand lambda expression. */
#define LAM(argument, expression)\
	LAM_EX(argument, return (expression);)

/* Forwards all arguments to func. */
#define FWD(func)\
	[&](auto&&...a) { return (func)(a...); }



/* Shorthand lambda taking two arguments, with function body. */
#define LAM2_EX(a0, a1, body)\
	[&](auto&& a0, auto&& a1){body}

/* Shorthand lambda taking two arguments. */
#define LAM2(a0, a1, expression)\
	LAM2_EX(a0, a1, return (expression);)



/* Shorthand lambda taking two arguments and returning a reference, with function body. */
#define LAM_PRINT_EX(a0, a1, body)\
	[&](auto&& a0, auto&& a1) -> auto& {body}

/* Shorthand lambda taking two arguments and returning a reference. */
#define LAM_PRINT(a0, a1, expression)\
	LAM_PRINT_EX(a0, a1, return (expression);)

/* Forwards all arguments to func, returning a reference. */
#define FWD_PRINT(func)\
	[&](auto&&...a) -> auto& { return (func)(a...); }
