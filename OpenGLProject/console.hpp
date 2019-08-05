
#pragma once

#include "console_message_queue.hpp"



namespace console
{
	// not thread-safe

	void printCriticalError(std::string const& message);



	void init();
	void destroy();



	// thread-safe



	void enqueMessage(ConsoleMessage const& message);

	inline void enqueMessage(ConsoleMessageType type, std::string const& message)
	{
		enqueMessage(ConsoleMessage{ type, message });
	}



	inline void error(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::Error, message);
	}

	inline void warning(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::Warning, message);
	}

	inline void info(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::Info, message);
	}

	inline void chat(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::Chat, message);
	}

	inline void log(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::Log, message);
	}
}
