
#include <fstream>
#include <sstream>
#include <iostream>

#include "config.hpp"



ReturnCode matchText(std::istream& ins, std::string const& text)
{
	std::string s;
	ins >> s;
	ASSERT1_RE(text == s, ("expected '" + text + "', got '" + s + "'."));
	return RC_SUCCESS;
}



template<class T>
ReturnCode readProperty(std::istream& ins, std::string const& name, T& out)
{
	BEGIN_ANYALL();

	DO_ANYALL(matchText(ins, name));
	DO_ANYALL(matchText(ins, "="));

	int c = ins.peek();
	while (isspace(c))
	{
		ins.get();
		c = ins.peek();
	}

	std::getline(ins, out, ';');

	//DO_ANYALL(matchText(ins, ";"));

	return END_ANYALL();
}



template<class T>
ReturnCode readPropertyLine(std::istream& ins, std::string const& name, T& out)
{
	std::string line;
	std::getline(ins, line);
	std::istringstream ss{ line };
	return readProperty(ss, name, out);
}



ReturnCode Config::load(Config& out)
{
	std::string const filePath = "./config.txt";
	std::ifstream file{ filePath };
	ASSERT1_RE(file, ("Failed to load file at " + filePath));

	BEGIN_ANYALL();

	ReturnCode c;

	c = readPropertyLine(file, "username", out.username);
	if (c) { printError(c); }
	DO_ANYALL(c);

	c = readPropertyLine(file, "servername", out.servername);
	if (c) { printError(c); }
	DO_ANYALL(c);

	ReturnCode const r = END_ANYALL();
	if (r) { *g_reason = "Failed to load all of config file."; }
	return r;
}
