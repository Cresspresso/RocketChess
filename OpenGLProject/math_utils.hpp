/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	math_utils.hpp
**
**	Summary		:	Math functions.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <cmath>
#include <algorithm>

// modulus for float
inline float mymod(float n, float d)
{
	return std::fmodf(n, d);
}

// modulus for double
inline double mymod(double n, double d)
{
	return std::fmod(n, d);
}

// modulus for integral types
template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
constexpr T mymod(T n, T d)
{
	return n % d;
}

// cycles in range [0, length)
template<class T>
T cycle(T x, T length) noexcept
{
	x = mymod(x, length);
	return x < 0 ? x + length : x;
}

// cycles in range [min, max)
template<class T>
T cycle(T x, T min, T max) noexcept
{
	return cycle(x - min, max - min) + min;
}

// clamps in range [min, max]
template<class T>
constexpr T clamp(T const& val, T const& min, T const& max) noexcept
{
	return std::clamp(val, min, max);
}
