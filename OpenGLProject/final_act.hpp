/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	final_act.hpp
**
**	Summary		:	Executes an action in the destructor.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <type_traits>



template<
	class Callback,

	class = std::enable_if_t<
		std::is_nothrow_invocable_v<Callback>
	>
>
	class FinalAct
{
#pragma region Public
public:



	// Lifetime



	FinalAct() = delete;
	FinalAct(FinalAct const&) = delete;
	FinalAct& operator=(FinalAct const&) = delete;
	FinalAct(FinalAct&&) = default;
	FinalAct& operator=(FinalAct&&) = default;



	~FinalAct() noexcept
	{
		action();
	}
	//~ dtor



	// Constructors



	explicit FinalAct(Callback&& action) : action{ std::forward<Callback>(action) } {}



#pragma endregion ~Public
#pragma region Private
private:



	// Fields



	Callback action{};



#pragma endregion ~Private
};
//~ class FinalAct



#define FINAL_ACT(x, ...)\
	auto x##fn = (__VA_ARGS__);\
	FinalAct<decltype(x##fn)> x{ std::move(x##fn) }

#define FINAL_ACT_LAM(x, ...) FINAL_ACT(x, ([&]()noexcept __VA_ARGS__))
