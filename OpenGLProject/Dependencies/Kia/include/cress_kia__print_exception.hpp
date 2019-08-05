/*
**	File		:	cress_kia__print_exception.hpp
**
**	Summary 	:	Functions for printing exceptions to streams.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

// std library includes
#include <iosfwd>

// local includes
#include "cress_kia_raw__types.hpp"
#include "cress_kia__type_conditionals.hpp"



#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia
{
	/*
	**	Function:	printException
	**
	**	Summary	:	Prints the current exception message,
	**				and any nested exception messages, to the stream.
	**
	**	Details	:	This Lippincott function rethrows the current exception,
	**				then catches the following types:
	**				1.	std::exception const&
	**				2.	std::nested_exception const&
	**				3.	cress::kia::raw::StringConstView (char const*)
	**				4.	int
	**				5.	... (any thrown object)
	**				It displays an error message appropriate to the type that is caught.
	**				The error message is prefixed with the |title|, in the format "title: ".
	**				Any nested exception messages are indented and prefixed with "Nested exception: ".
	**
	**	Param	:	outs
	**				<std::ostream&>
	**				Output stream.
	**
	**	Param	:	indentDepth
	**				<size_t>
	**				Number of indentation tabs to print before the message.
	**
	**	Param	:	title
	**				<cress::kia::raw::StringConstView>
	**				Title of the message.
	**
	**	Returns	:	<std::ostream&>
	**				Param |outs|.
	**
	**	Example:
		*
		*	try
		*	{
		*		throw std::logic_error("my error");
		*	}
		*	catch (...)
		*	{
		*		std::cout << "Pre-message\n";
		*		cress::kia::printException(std::cout, 1, "Error title");
		*	}
		*
	**	Output:
		*
		*	Pre-message
		*		Error title: my error
		*
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	extern std::ostream&
	printException(
		std::ostream& outs,
		size_t indentDepth = 0,
		cress::kia::raw::StringConstView title = "Exception"
	) noexcept;
	//~ fn printException



	/*
	**	Function:	printExceptionF
	**
	**	Summary	:	Prints the current exception message,
	**				and any nested exception messages, to the stream.
	**
	**	Details	:	This Lippincott function rethrows the current exception,
	**				then catches the following types:
	**				1.	std::exception const&
	**				2.	std::nested_exception const&
	**				3.	cress::kia::raw::StringConstView (char const*)
	**				4.	int
	**				5.	... (any thrown object)
	**				It displays an error message appropriate to the type that is caught.
	**				The error message is prefixed with "Exception: ".
	**				Any nested exception messages are indented and prefixed with "Nested exception: ".
	**
	**	Param	:	outs
	**					<std::ostream&>
	**					Output stream.
	**				
	**	Returns	:	<std::ostream&>
	**					Param |outs|.
	**
	**	Example:
		*
		*	try
		*	{
		*		throw std::logic_error("my error");
		*	}
		*	catch (...)
		*	{
		*		std::cout << cress::kia::printExceptionF;
		*	}
		*
	**	Output:
		*
		*	Exception: my error
		*
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	06/04/2019
	*/
	inline std::ostream&
	printExceptionF(std::ostream& outs) noexcept
	{
		return printException(outs, 0, "Exception");
	}
	//~ fn printException



	/*
	**	Function:	tryDo
	**
	**	Summary :	Tries to execute an action.
	**				If it throws an exception, logs the exception message
	**				to the error stream.
	**
	**	TParam	:	Action
	**				<() -> void>
	**				See Param |action|.
	**
	**	Param	:	action
	**				<() -> void>
	**				Callable object that should be executed.
	**
	**	Param	:	errorStream
	**				<std::ostream&>
	**				Output stream for error messages.
	**
	**	Param	:	indentDepth
	**				<size_t>
	**				Number of indentation tabs to print before the error message.
	**
	**	Param	:	title
	**				<cress::kia::raw::StringConstView>
	**				Title of the error message.
	**
	**	Example:
		*
		*	bool const b = true;
		*	tryDo([b]
		*		{
		*			if (b) { throw std::logic_error("my error"); }
		*			std::cout << "Success.\n";
		*		},
		*		std::cerr
		*	);
		*	std::cout << "Done.\n";
		*
	**	Output to std::cerr:
		*
		*	Exception: my error
		*	Done.
		*
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	06/04/2019
	*/
	TM<CL Action> void
	tryDo(
		Action&& action,
		std::ostream& errorStream,
		size_t indentDepth = 0,
		cress::kia::raw::StringConstView title = "Exception"
	) NE
		try { std::forward<Action>(action)(); }
		catch (...)
		{
			try { printException(errorStream, indentDepth, title); }
			catch (...) {}
		}
	//~ fn tryDo



	/*
	**	Function:	tryGet
	**
	**	Summary :	Tries to execute a function and return the result.
	**				If it throws an exception, logs the exception message
	**				to the error stream, then returns a default value.
	**
	**	TParam	:	GetterFunc
	**				<() -> Retval>
	**				See Param |getterFunc|.
	**
	**	TParam	:	RetvalOnError
	**				See Param |retvalOnError|.
	**
	**	TParam	:	Retval
	**				Return type of TParam |GetterFunc|.
	**
	**	Requires:	RetvalOnError must be convertible to Retval.
	**
	**	Param	:	getterFunc
	**				<() -> Retval>
	**				Callable object that should be executed
	**				in order to return its result value.
	**
	**	Param	:	retvalOnError
	**				<RetvalOnError>
	**				Value returned when an exception is caught.
	**
	**	Param	:	errorStream
	**				<std::ostream&>
	**				Output stream for error messages.
	**
	**	Param	:	indentDepth
	**				<size_t>
	**				Number of indentation tabs to print before the error message.
	**
	**	Param	:	title
	**				<cress::kia::raw::StringConstView>
	**				Title of the error message.
	**
	**	Example:
		*
		*	bool const b = true;
		*	int const val = tryGet([b]() -> int
		*		{
		*			if (b) { throw std::logic_error("my error"); }
		*			return 0;
		*		},
		*		-1,
		*		std::cerr
		*	);
		*	std::cout << "val == " << val << '\n';
		*
	**	Output:
		*
		*	Exception: my error
		*	val == -1
		*
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	06/04/2019
	*/
	TM<
		CL GetterFunc,
		CL RetvalOnError,
		CL Retval = std::invoke_result_t<GetterFunc>,
		CL = EnableIf<std::is_convertible_v<RetvalOnError&&, Retval>>
	>
	Retval
	tryGet(
		GetterFunc&& getterFunc,
		RetvalOnError&& retvalOnError,
		std::ostream& errorStream,
		size_t indentDepth = 0,
		cress::kia::raw::StringConstView title = "Exception"
	) NE
		try { return std::forward<GetterFunc>(getterFunc)(); }
		catch (...)
		{
			try { printException(errorStream, indentDepth, title); }
			catch (...) {}
			
			return std::forward<RetvalOnError>(retvalOnError);
		}
	//~ fn tryGet



	/*
	**	Function:	tryGet
	**
	**	Summary :	Tries to execute a function and return the result.
	**				If it throws an exception, logs the exception message
	**				to the error stream, then returns a default value.
	**
	**	TParam	:	GetterFunc
	**				<() -> Retval>
	**				See Param |getterFunc|.
	**
	**	TParam	:	RetvalOnError
	**				See Param |retvalOnError|.
	**
	**	TParam	:	Retval
	**				Return type of TParam |GetterFunc|.
	**
	**	Requires:	RetvalOnError must be convertible to Retval.
	**
	**	Param	:	getterFunc
	**				<() -> Retval>
	**				Callable object that should be executed
	**				in order to return its result value.
	**
	**	Param	:	retvalOnError
	**				<RetvalOnError>
	**				Value returned when an exception is caught.
	**
	**	Example:
		*
		*	bool const b = true;
		*	int const val = tryGet([b]() -> int
		*		{
		*			if (b) { throw std::logic_error("my error"); }
		*			return 0;
		*		},
		*		-1
		*	);
		*	std::cout << val << '\n';
		*
	**	Output:
		*
		*	-1
		*
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	06/04/2019
	*/
	TM<
		CL GetterFunc,
		CL RetvalOnError,
		CL Retval = std::invoke_result_t<GetterFunc>,
		CL = EnableIf<std::is_convertible_v<RetvalOnError&&, Retval>>
	>
	Retval
	tryGet(
		GetterFunc&& getterFunc,
		RetvalOnError&& retvalOnError
	) NE
		try { return std::forward<GetterFunc>(getterFunc)(); }
		catch (...) { return std::forward<RetvalOnError>(retvalOnError); }
	//~ fn tryGet
}
//~ namespace cress::kia



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
