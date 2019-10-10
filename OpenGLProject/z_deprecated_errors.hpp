/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	errors.hpp
**
**	Summary 	:	Error propagating and handling functions.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once
#ifdef 0

#include <cassert>
#include <memory>
#include <string>
#include <Kia/include/cress_kia__print_exception.hpp>



enum ReturnCodde
{
	RC_SUCCESS = 0,
	RC_ERROR = 1,
	RC_PARTIAL = 2,
};



template<class T>
ReturnCodde innerAssert0(T const& c)
{
	if (c)
	{
		//assert(0); // for debugging
		return RC_ERROR;
	}
	return RC_SUCCESS;
}



// sets reason message and returns RC_ERROR immediately.
#define FAIL_RE(...) do { *g_reason = (__VA_ARGS__); return RC_ERROR; } while(0)

// returns RC_ERROR immediately if condition is not 0.
#define ASSERT0(...) do { if (innerAssert0(__VA_ARGS__)) { return RC_ERROR; } } while(0)
#define ASSERT1(...) ASSERT0(!(__VA_ARGS__))

// sets reason message and returns RC_ERROR immediately if condition is not 0.
#define ASSERT0_RE(C, ...) do { if (innerAssert0(C)) { FAIL_RE(__VA_ARGS__); } } while (0)
#define ASSERT1_RE(C, ...) ASSERT0_RE(!(C), (__VA_ARGS__))



// reason for an error (not thread safe)
extern std::unique_ptr<std::string> g_reason;

// prints g_reason
std::string stringError(int code);
// rethrows current exception and prints it
std::string stringException();

void printError(int r);
void printException();



#define CATCH_RE() catch (...) { FAIL_RE(stringException()); } do{}while(0)
#define CATCH_PRINT() catch (...) { printException(); } do{}while(0)

#define TRY_CATCH_RE(...) try { (__VA_ARGS__); } CATCH_RE()
// invokes an expression that returns void, and prints any exceptions.
#define TRY_CATCH_PRINT(...) try { (__VA_ARGS__); } CATCH_PRINT()

// invokes an expression that returns an error code, and prints any errors or exceptions.
#define HANDLE_ALL(...)\
	try {\
		auto const e = (__VA_ARGS__);\
		if (e) { printError(e); }\
	} CATCH_PRINT()



#define BEGIN_ANYALL()	bool any__ = false; bool all__ = true; do{} while(0)
#define DO_ANYALL(...)	do { if (__VA_ARGS__) { all__ = false; } else { any__ = true; } } while(0)
#define END_ANYALL()	(any__ ? (all__ ? RC_SUCCESS : RC_PARTIAL) : RC_ERROR)
#endif