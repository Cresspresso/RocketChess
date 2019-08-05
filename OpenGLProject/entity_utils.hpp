/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	entity_utils.hpp
**
**	Summary		:	Utility functions for entities.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#ifdef COMMENT
#pragma once

#include "common.hpp"

template<class T>
ReturnCode updateOne(T& p)
{
	return p ? p->update() : RC_ERROR;
}

template<class T>
ReturnCode renderOne(T& p)
{
	return p ? p->render() : RC_ERROR;
}

template<class C>
ReturnCode updateAll(C& v)
{
	for (auto* p : v)
	{
		if (!p) {
			continue;
		}
		HANDLE_ALL(p->update());
	}

	return RC_SUCCESS;
}

template<class C>
ReturnCode renderAll(C& v)
{
	for (auto* p : v)
	{
		if (!p) {
			continue;
		}
		HANDLE_ALL(p->render());
	}

	return RC_SUCCESS;
}
#endif //~ COMMENT