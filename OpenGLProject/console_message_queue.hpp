
#pragma once

#include <queue>

#include "work_queue.hpp"

#include "errors.hpp"



enum ConsoleMessageType
{
	Error,
	Warning,
	Info,
	Chat,
	Log,
	DebugLog,
};



struct ConsoleMessage
{
	ConsoleMessageType type;
	std::string content;
};



extern std::mutex g_ConsoleMutex;



// thread-safe
void printConsoleMessage(ConsoleMessage const& message);



class ConsoleMessageQueue
{
public:

	// thread-safe
	void enqueMessage(ConsoleMessage const& message);
	void processMessages();

	// not thread-safe
	void init(); 
	void destroy();

private:

	// not thread-safe
	void assertQueueNotNull() noexcept(false);

	// variables
	std::mutex m_MessagesMutex;
	std::unique_ptr<std::queue<ConsoleMessage>> m_MessagesQueue;
};
