/*
**	File		:	cress_kia__exception.hpp
**
**	Summary 	:	Base exception class for Kia, inheriting from |std::exception|.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include <stdexcept>

#include "cress_kia__macros.hpp"
#include "cress_kia_raw__types.hpp"

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia
{
	struct KiaException : public std::exception
	{
		using Super = std::exception;


		CRESS_KIA__DEF_CTORS_DEFAULTED(KiaException);
		virtual ~KiaException() = default;

		EX KiaException(raw::StringView const msg) NE
			: Super{ msg }
		{}

		struct OwnerTag {};
		EX KiaException(raw::StringOwner const msg, OwnerTag) NE
			: Super{ msg, 1 }
		{}
	};
	//~ struct KiaException
}
//~ namespace cress::kia



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
