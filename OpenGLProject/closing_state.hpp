
#pragma once

#include <atomic>

enum class ClosingState
{
	Running,			// During and after main initialisation, and before the glut window or the console is closed.
	WantingToClose,		// If the console window was manually closed. ::close() should be called soon.
	Closing,			// When ::close() is in progress.
	Finished,			// After ::close() has finished.
};

extern std::atomic<ClosingState> g_closing;
