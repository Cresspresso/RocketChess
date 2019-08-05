
#pragma once

#include <string>

#include "errors.hpp"

struct Config
{
	std::string username = "unknown";
	std::string servername = "unknown";

	static ReturnCode load(Config& out);
};
