
#include <cassert>

#include <Kia/include/cress_kia__print_exception.hpp>

#include "to_string.hpp"
#include "console.hpp"

#include "exceptions.hpp"



std::string stringException()
{
	return stringLink(cress::kia::printExceptionF);
}

void printException()
{
	std::string const msg = stringException();
	console::error(msg);
#ifdef _DEBUG
	assert(0);
#endif
}

void printErrorCode(int code)
{
	std::string const msg = stringLink("Error Code: ", code);
	console::error(msg);
#ifdef _DEBUG
	assert(0);
#endif
}
