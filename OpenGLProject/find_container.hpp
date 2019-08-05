
#pragma once

#include <algorithm>

namespace container
{
	template<class T, class C>
	bool has(C const& v, T const& item)
	{
		using std::begin;
		using std::end;
		auto const e = end(v);
		return e != std::find(begin(v), e, item);
	}

	template<class C, class P>
	bool hasIf(C const& v, P pred)
	{
		using std::begin;
		using std::end;
		auto const e = end(v);
		return e != std::find_if(begin(v), e, pred);
	}

	template<class C, class P>
	void removeAndEraseIf(C& v, P pred)
	{
		using std::begin;
		using std::end;
		auto const e = end(v);
		auto const it = std::remove_if(begin(v), e, pred);
		v.erase(it, e);
	}

	// Source: https://stackoverflow.com/questions/800955/remove-if-equivalent-for-stdmap
	template<typename ContainerT, typename PredicateT>
	void eraseIf(ContainerT& items, const PredicateT& predicate)
	{
		using std::begin;
		using std::end;
		auto const e = end(items);
		for (auto it = begin(items); it != e; )
		{
			if (predicate(*it)) it = items.erase(it);
			else ++it;
		}
	}
}
