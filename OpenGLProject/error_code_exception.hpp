
#pragma once

#include <stdexcept>
#include <string>

#include "to_string.hpp"



template<class Code>
class ErrorCodeException : public std::runtime_error
{
public:
	ErrorCodeException(Code const& code, std::string const& title = "ErrorCodeException")
		: std::runtime_error{ stringLink(title, ": ", toString(code)) },
		code{ code }
	{}



	Code const code;
};
