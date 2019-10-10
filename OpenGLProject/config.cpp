
#include <fstream>
#include <sstream>
#include <iostream>

#include "config.hpp"



void matchText(std::istream& ins, std::string const& text)
{
	std::string s;
	ins >> s;
	if (text != s) { throw std::runtime_error("expected '" + text + "', got '" + s + "'."); }
}



template<class T>
void readProperty(std::istream& ins, std::string const& name, T& out)
{
	matchText(ins, name);
	matchText(ins, "=");

	int c = ins.peek();
	while (isspace(c))
	{
		ins.get();
		c = ins.peek();
	}

	std::getline(ins, out, ';');

	//matchText(ins, ";");
}



template<class T>
void readPropertyLine(std::istream& ins, std::string const& name, T& out)
{
	std::string line;
	std::getline(ins, line);
	std::istringstream ss{ line };
	readProperty(ss, name, out);
}



void Config::load(Config& out)
{
	std::string const filePath = "./config.txt";
	std::ifstream file{ filePath };
	if (!file) { throw std::runtime_error("Failed to load file at " + filePath); }

	bool failure = false;

	try { readPropertyLine(file, "username", out.username); }
	catch (...) { failure = true; }

	try { readPropertyLine(file, "servername", out.servername); }
	catch (...) { failure = true; }

	if (failure) { throw std::runtime_error("Not all properties were read correctly."); }
}
