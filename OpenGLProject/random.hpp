/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	random.hpp
**
**	Summary 	:	Random Number Generators.
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/

#pragma once

#include <random>



/*
**	Variable	:	g_rng
**
**	Summary 	:	Engine for random number generators.
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/
extern std::default_random_engine g_rng;

//inline std::default_random_engine makeRng() { return std::default_random_engine{ std::random_device{}() }; }



/*
**	Function	:	randomInt
**
**	Summary 	:	Generates random integer number in range [min, max].
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/
template<class T = int>
T randomInt(T min, T max)
{
	return std::uniform_int_distribution<T>{ min, max }(g_rng);
}



/*
**	Function	:	randomIndex
**
**	Summary 	:	Generates random integer number in range [0, size - 1].
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/
template<class T = size_t>
T randomIndex(T size)
{
	return std::uniform_int_distribution<T>{ 0, size - 1 }(g_rng);
}



/*
**	Function	:	randomRealEx
**
**	Summary 	:	Generates random floating point real number in range [min, max).
**
**	Project		:	Summative3
**	Class		:	GD2P01
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	22/05/2019
*/
template<class T = float>
T randomRealEx(T min, T max)
{
	return std::uniform_real_distribution<T>{ min, max }(g_rng);
}
