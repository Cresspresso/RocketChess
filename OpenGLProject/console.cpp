
#include <iostream>

#include <Kia/include/cress_kia__print_exception.hpp>

#include "closing_state.hpp"
#include "console_color.hpp"
#include "to_string.hpp"
#include "compatibility_threading.hpp"

#include "console.hpp"



namespace console
{
	void printCriticalError(std::string const& message)
	{
		lock_guard lock{ g_ConsoleMutex };
		setConsoleTextColor(ConsoleForegroundColor::LightRed);
		std::cerr << "\n[!!!] Critical Error: " << message << '\n';

#ifdef _DEBUG
		assert(0);
#endif
	}



	namespace
	{
		static ConsoleMessageQueue messageQueue;
		static std::unique_ptr<std::thread> thread = nullptr;



		void process()
		{
			while (ClosingState::Running == g_closing)
			{
				try
				{
					messageQueue.processMessages();
				}
				catch (...)
				{
					printCriticalError(stringLink("Console: ", cress::kia::printExceptionF));
				}
			}
		}
	}



	void init()
	{
		messageQueue.init();
		thread = std::make_unique<std::thread>(process);
	}



	void destroy()
	{
		if (thread)
		{
			thread->join();
			thread.reset();
		}

		// after joining its thread
		messageQueue.destroy();


		setConsoleTextColor(ConsoleForegroundColor::LightGray);
	}
	


	void enqueMessage(ConsoleMessage const& message)
	{
		messageQueue.enqueMessage(message);
	}
}
