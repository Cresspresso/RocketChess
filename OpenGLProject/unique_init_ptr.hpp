/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	unique_init_ptr.hpp
**
**	Summary 	:	Wrapper for std::unique_ptr with init and destroy capabilities.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <memory>

template<class T>
struct UniqueInitPtr
{
	std::unique_ptr<T> p = nullptr;

	auto initPtr()
	{
		p = std::make_unique<T>();
		return p->init();
	}
	void destroyPtr() noexcept
	{
		p->destroy();
		p.reset();
	}

	T& operator*() noexcept { return *p; }
	T const& operator*() const noexcept { return *p; }
	T* operator->() noexcept { return p.get(); }
	T const* operator->() const noexcept { return p.get(); }
};
