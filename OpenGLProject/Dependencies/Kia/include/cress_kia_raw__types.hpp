/*
**	File		:	cress_kia_raw__types.hpp
**
**	Summary 	:	Type aliases for C++ raw unchecked types.
**
**	Project		:	Kia
**	License		:	See LICENSE.txt
**	Author		:	Elijah Shadbolt (Cresspresso)
**	Email		:	cresspresso@gmail.com
**	Date Edited	:	02/04/2019
*/

#pragma once

#include <cstdint>

#pragma region Shorthand
#include "cress_kia__def_shorthand.hpp"



namespace cress::kia::raw
{
#pragma region Signed Integrals

	using SByte = signed char;
	using Short = signed short;
	using Int = signed int;
	using Long = signed long;
	using LLong = signed long long;

	using Int8 = int8_t;
	using Int16 = int16_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	using IntFast8 = int_fast8_t;
	using IntFast16 = int_fast16_t;
	using IntFast32 = int_fast32_t;
	using IntFast64 = int_fast64_t;

#pragma endregion ~Signed Integrals
#pragma region Unsigned Integrals

	using UByte = unsigned char;
	using UShort = unsigned short;
	using UInt = unsigned int;
	using ULong = unsigned long;
	using ULLong = unsigned long long;

	using UInt8 = uint8_t;
	using UInt16 = uint16_t;
	using UInt32 = uint32_t;
	using UInt64 = uint64_t;

	using UIntFast8 = uint_fast8_t;
	using UIntFast16 = uint_fast16_t;
	using UIntFast32 = uint_fast32_t;
	using UIntFast64 = uint_fast64_t;

#pragma endregion ~Unsigned Integrals
#pragma region Chars

	using Char = char;
	using CharWide = wchar_t;

	using Char8 = char;
	using Char16 = char16_t;
	using Char32 = char32_t;

#pragma endregion ~Chars
#pragma region Miscellaneous

	/** [[deprecated("Use |bool| primitive type keyword.")]] */
	using Bool = bool;

	/** [[deprecated("Use |std::size_t| type alias.")]] */
	using Size = size_t;

	/** [[deprecated("Use |std::ptrdiff_t| type alias.")]] */
	using PtrDiff = ptrdiff_t;
	/** [[deprecated("Use |std::intptr_t| type alias.")]] */
	using IntPtr = intptr_t;

#pragma endregion ~Miscellaneous
#pragma region References

	TM1 using Reference = T&;
	TM1 using RValueReference = T&&;

#pragma endregion ~References
#pragma region Ownership

	TM<CL Ptr>
	using Viewing = Ptr;

	TM<CL Ptr>
	using Owning = Ptr;

#pragma region ~Ownership
#pragma endregion Pointers

	TM1 using Pointer = T*;

	TM1 using SinglePtr = Pointer<T>;
	TM1 using Viewer = Viewing<SinglePtr<T>>;
	TM1 using Owner = Owning<SinglePtr<T>>;

	TM1 using UnboundedArrayPtr = Pointer<T>;
	TM1 using UnboundedArrayView = Viewing<UnboundedArrayPtr<T>>;
	TM1 using UnboundedArrayOwner = Owning<UnboundedArrayPtr<T>>;

#pragma endregion ~Pointers
#pragma region C-Strings

	TM<CL CharT>
	using CStringView = UnboundedArrayView<CharT>;

	using StringView = CStringView<Char>;
	using StringConstView = CStringView<Char $>;
	using WideStringView = CStringView<CharWide>;
	using WideStringConstView = CStringView<CharWide $>;

	TM<CL CharT>
	using CStringOwner = UnboundedArrayOwner<CharT>;

	using StringOwner = CStringOwner<Char>;
	using StringConstOwner = CStringOwner<Char $>;
	using WideStringOwner = CStringOwner<CharWide>;
	using WideStringConstOwner = CStringOwner<CharWide $>;

#pragma endregion ~C-Strings
}
//~ namespace cress::kia::raw



#include "cress_kia__undef_shorthand.hpp"
#pragma endregion ~Shorthand
