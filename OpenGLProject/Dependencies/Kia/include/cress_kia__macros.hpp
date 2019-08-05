/*
**	File		:	cress_kia__macros.hpp
**
**	Summary 	:	General utility macros.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once



#pragma region Miscellaneous



#define CRESS_KIA__GROUP(...) __VA_ARGS__
#define CRESS_KIA__STRINGIFY_TOKEN(X) #X
#define CRESS_KIA__STRINGIFY(X) CRESS_KIA__STRINGIFY_TOKEN(X)
#define CRESS_KIA__CONCAT(A, B) A##B



#pragma endregion ~Miscellaneous
#pragma region Attributes



#define CRESS_KIA__NODISCARD [[nodiscard]]



// Implicit conversion keyword for constructors (like "explicit").
#define CRESS_KIA__IMPLICIT



#pragma endregion ~Attributes
#pragma region Macro_CRESS_KIA__MUST_NOT_COMPILE()



#ifdef CRESS_KIA_PPARG__TEST_MUST_NOT_COMPILE

	// Indicates the expression must not compile.
	#define CRESS_KIA__MUST_NOT_COMPILE(Reason, ...) static_assert([]()constexpr{{__VA_ARGS__;}return false;},"This must not compile.");

#else //! ifdef CRESS_KIA_PPARG__TEST_MUST_NOT_COMPILE

	// Indicates the expression must not compile.
	#define CRESS_KIA__MUST_NOT_COMPILE(Reason, ...)

#endif //~ ifdef CRESS_KIA_PPARG__TEST_MUST_NOT_COMPILE



#pragma endregion ~Macro_CRESS_KIA__MUST_NOT_COMPILE()
#pragma region Lifetime Constructors



#define CRESS_KIA__DEF_DTOR(Class) ~Class() noexcept
#define CRESS_KIA__DEF_DEFAULT_CTOR(Class) Class() noexcept
#define CRESS_KIA__DEF_COPY_CTOR(Class) Class(Class const& that)
#define CRESS_KIA__DEF_MOVE_CTOR(Class) Class(Class&& that) noexcept
#define CRESS_KIA__DEF_COPY_OP(Class) Class& operator=(Class const& that)
#define CRESS_KIA__DEF_MOVE_OP(Class) Class& operator=(Class&& that) noexcept
#define CRESS_KIA__DEF_SWAP(Class) void swap(Class& that) noexcept

#define CRESS_KIA__DEF_FRIEND_SWAP(Class)\
	void swap(Class& a, Class& b) noexcept { a.swap(b); }

#define CRESS_KIA__DEF_CTORS_DEFAULTED(Class)\
	CRESS_KIA__DEF_DEFAULT_CTOR(Class) = default;\
	CRESS_KIA__DEF_COPY_CTOR(Class) = default;\
	CRESS_KIA__DEF_MOVE_CTOR(Class) = default;\
	CRESS_KIA__DEF_COPY_OP(Class) = default;\
	CRESS_KIA__DEF_MOVE_OP(Class) = default

#define CRESS_KIA__DEF_LIFETIME_DEFAULTED(Class)\
	CRESS_KIA__DEF_CTORS_DEFAULTED(Class);\
	CRESS_KIA__DEF_DTOR(Class) = default
	

#define CRESS_KIA__DEF_MOVEABLE_DEFAULTED(Class)\
	CRESS_KIA__DEF_MOVE_CTOR(Class) = default;\
	CRESS_KIA__DEF_MOVE_OP(Class) = default;\
	CRESS_KIA__DEF_COPY_CTOR(Class) = delete;\
	CRESS_KIA__DEF_COPY_OP(Class) = delete



#pragma endregion ~Lifetime Constructors
