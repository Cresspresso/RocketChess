/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/moo/final_act.hpp
//
//	Summary		:	Executes an action in the destructor.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	19/09/2019
*/

#pragma once

#include <type_traits>

namespace cress
{
	namespace moo
	{
		/*
		//	FinalAct
		//	Executes an action in the destructor.
		//
		//	Example:

			cress::FinalAct fa{ [&]()noexcept { cleanupStuff(); } };

		//	Example 2:

			CRESS_FINAL_ACT(fa, ([&]()noexcept { cleanupStuff(); }));

		//	Example 3:

			CRESS_FINAL_ACT_SINGLE(fa, cleanupStuff());

		//	Example 4:

			CRESS_FINAL_ACT_BEGIN(fa);
			cleanupStuff();
			cleanupOtherStuff();
			CRESS_FINAL_ACT_END(fa);

		*/
		template<class Callback,
			class = std::enable_if_t<std::is_nothrow_invocable_v<Callback>>
		> class FinalAct
		{
		public:
			~FinalAct() noexcept { action(); }
			FinalAct(FinalAct&&) = default;
			FinalAct& operator=(FinalAct&&) = default;

			FinalAct() = delete;
			FinalAct(FinalAct const&) = delete;
			FinalAct& operator=(FinalAct const&) = delete;

			explicit FinalAct(Callback&& action) : action{ std::forward<Callback>(action) } {}

		private:
			Callback action{};
		};
	}
}



#define CRESS_MOO_FINAL_ACT(name, callable)\
	auto name##fn = (callable);\
	::cress::moo::FinalAct<decltype(name##fn)> name{ ::std::move(name##fn) }

#define CRESS_MOO_FINAL_ACT_SINGLE(name, expression)\
	CRESS_MOO_FINAL_ACT(name, ([&]()noexcept{ (expression); }))

#define CRESS_MOO_FINAL_ACT_BEGIN(name)\
	auto name##fn = ([&]()noexcept { do{}while(0)

#define CRESS_MOO_FINAL_ACT_END(name)\
	});\
	::cress::moo::FinalAct<decltype(name##fn)> name{ ::std::move(name##fn) }
