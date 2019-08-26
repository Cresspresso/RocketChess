/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	resource_manager.hpp
**
**	Summary		:	Mixin base class for a resource manager.
**					Uses an enum indexer to represent available resources.
**					Static number of resources.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <array>

template<class T, size_t N, class Indexer_ = size_t>
struct ResourceManager /* abstract, with non-virtual destructor */
{
	using Indexer = Indexer_;
	using Resource = T;



	std::array<T, N> items{};

	T& operator[](size_t index) noexcept
	{
		return items[index];
	}
	T const& operator[](size_t index) const noexcept
	{
		return items[index];
	}

	T& operator[](Indexer_ index) noexcept
	{
		return items[static_cast<size_t>(index)];
	}
	T const& operator[](Indexer_ index) const noexcept
	{
		return items[static_cast<size_t>(index)];
	}



	virtual void destroyResource(Resource& resource) noexcept = 0;

	void destroy() noexcept
	{
		for (T& x : items)
		{
			destroyResource(x);
		}
	}

	virtual ReturnCode load(Resource& out, size_t i) = 0;

	ReturnCode init()
	{
		// for each indexer, load the corresponding resource, in order.
		size_t numLoaded = 0;
		for (size_t i = 0; i < N; ++i)
		{
			// try to load the resource and increase loaded count
			try
			{
				ReturnCode r = load(items[i], i);
				if (r)
				{
					printError(r);
				}
				else
				{
					++numLoaded;
				}
			}
			CATCH_PRINT();
		}

		if (numLoaded == N) { return RC_SUCCESS; }
		else if (numLoaded == 0) { return RC_ERROR; }
		else { return RC_PARTIAL; }
	}
};
