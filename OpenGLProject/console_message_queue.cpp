
#include <iostream>

#include "console_color.hpp"
#include "to_string.hpp"
#include "compatibility_threading.hpp"

#include "console_message_queue.hpp"



std::mutex g_ConsoleMutex;



void printConsoleMessage(ConsoleMessage const& message)
{
	ConsoleForegroundColor color = ConsoleForegroundColor::LightGray;
	std::string formattedMessage;
	std::ostream* outs = nullptr;

	switch (message.type)
	{
	case ConsoleMessageType::Error:
	{
		outs = &std::cerr;
		color = ConsoleForegroundColor::LightRed;
		formattedMessage = stringLink("[!] ", message.content, '\n');
		break;
	}
	case ConsoleMessageType::Warning:
	{
		outs = &std::cerr;
		color = ConsoleForegroundColor::LightYellow;
		formattedMessage = stringLink("[!] ", message.content, '\n');
		break;
	}
	case ConsoleMessageType::Info:
	{
		outs = &std::cout;
		color = ConsoleForegroundColor::LightGray;
		formattedMessage = stringLink("[-] ", message.content, '\n');
		break;
	}
	case ConsoleMessageType::Chat:
	{
		outs = &std::cout;
		color = ConsoleForegroundColor::LightGreen;
		formattedMessage = stringLink("> ", message.content, '\n');
		break;
	}
	case ConsoleMessageType::Log:
	{
		outs = &std::clog;
		color = ConsoleForegroundColor::DarkYellow;
		formattedMessage = stringLink("[?] ", message.content, '\n');
		break;
	}
	case ConsoleMessageType::DebugLog:
	{
#ifdef _DEBUG
		outs = &std::clog;
		color = ConsoleForegroundColor::DarkCyan;
		formattedMessage = stringLink("[?] ", message.content, '\n');
#else //! _DEBUG
		return;
#endif //~ _DEBUG
		break;
	}
	default:
		throw std::runtime_error{ "invalid MessageType enum value" };
	}

	assert(outs);
	lock_guard lock{ g_ConsoleMutex };
	setConsoleTextColor(color);
	(*outs) << formattedMessage;
}



void ConsoleMessageQueue::enqueMessage(ConsoleMessage const& message)
{
	lock_guard lock{ m_MessagesMutex };
	assertQueueNotNull();
	m_MessagesQueue->push(message);
}



void ConsoleMessageQueue::processMessages()
{
	// Pop a copy of the queue of messages.
	unique_lock lk{ m_MessagesMutex };
	assertQueueNotNull();
	auto queue = std::move(*m_MessagesQueue);
	assert(m_MessagesQueue->empty());
	lk.unlock();

	// Print all messages in the queue.
	while (!queue.empty())
	{
		printConsoleMessage(std::move(queue.front()));
		queue.pop();
	}
}



void ConsoleMessageQueue::init()
{
	lock_guard lock{ m_MessagesMutex };
	m_MessagesQueue = std::make_unique<std::queue<ConsoleMessage>>();
}



void ConsoleMessageQueue::destroy()
{
	lock_guard lock{ m_MessagesMutex };
	m_MessagesQueue.reset();
}



void ConsoleMessageQueue::assertQueueNotNull() noexcept(false)
{
	if (!m_MessagesQueue)
	{
		throw std::runtime_error{ "Console Message Queue is null" };
	}
}
