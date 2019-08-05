/*
**	File		:	cress_kia__input_utils.hpp
**
**	Summary 	:	Basic console input prompts.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	03/04/2019
*/

#pragma once

// std library includes
#include <iosfwd>
#include <string_view>

// local includes
#include "cress_kia_raw__types.hpp"



namespace cress::kia
{
	/*
	**	Function	:	clearInputStream
	**
	**	Summary 	:	Clears the input stream and ignores all following characters.
	**					Use this function after a failed user input.
	**
	**	Param	ins	<std::istream&>	:	Input stream to clear.
	**	Returns		<std::istream&>	:	Param |ins|.
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	extern std::istream&
	clearInputStream(std::istream& ins);


	/*
	**	Function	:	askYesNo
	**
	**	Summary		:	Asks the user to input yes or no (y/n).
	**
	**	Details		:	Displays the prompt message "msg (y/n): ",
	**					then receives input, then validates it.
	**					If the input was not valid, an error message is
	**					displayed and the prompt is repeated.
	**
	**	Param	msg		<std::string_view>	:	Prompt message.
	**	Param	outs	<std::ostream&>		:	Output stream for prompt messages and error messages.
	**	Param	ins		<std::istream&>		:	Input stream (user terminal input).
	**	Returns			<bool>				:	True if they answered yes, false if they answered no.
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	extern bool
	askYesNo(
		std::string_view msg,
		std::ostream& outs,
		std::istream& ins
	);
	//~ fn askYesNo


	/*
	**	Function	:	askIntInRange
	**
	**	Summary		:	Asks the user to input an integer in the range [min, max].
	**
	**	Details		:	Displays the prompt message "msg [min, max]: ",
	**					then receives input, then validates it.
	**					If the input was not valid, an error message is
	**					displayed and the prompt is repeated.
	**
	**	Param	min		<cress::kia::raw::Int>	:	Minimum value, inclusive.
	**	Param	max		<cress::kia::raw::Int>	:	Maximum value, inclusive.
	**	Param	msg		<std::string_view>		:	Prompt message.
	**	Param	outs	<std::ostream&>			:	Output stream for prompt messages and error messages.
	**	Param	ins		<std::istream&>			:	Input stream (user terminal input).
	**	Returns			<cress::kia::raw::Int>	:	Value that the user entered, in the range [min, max].
	**
	**	Project		:	Kia
	**	License		:	See LICENSE.txt
	**	Author		:	Elijah Shadbolt (Cresspresso)
	**	Email		:	cresspresso@gmail.com
	**	Date Edited	:	03/04/2019
	*/
	extern cress::kia::raw::Int
	askIntInRange(
		cress::kia::raw::Int min,
		cress::kia::raw::Int max,
		std::string_view msg,
		std::ostream& outs,
		std::istream& ins
	);
	//~ fn askIntInRange
}
//~ namespace cress::kia
