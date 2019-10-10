
#pragma once

#include <string>

struct Config
{
	std::string username = "unknown";
	std::string servername = "unknown";

	static void load(Config& out);
};
