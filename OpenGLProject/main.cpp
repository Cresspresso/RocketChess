/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	main.cpp
**
**	Summary 	:	Application entry point.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

// Library Includes
//#include <vld.h>

#include <thread>
#include <atomic>

#include <Windows.h>

// Local Includes
#include "globals.hpp"
#include "world_math.hpp"

#include "application.hpp"
#include "singleton.hpp"

#include "time.hpp"
#include "screen.hpp"
#include "input.hpp"
#include "audio.hpp"
#include "toggle_music.hpp"

#include "network.hpp"
#include "socket.hpp"
#include "config.hpp"

#include "closing_state.hpp"
#include "console.hpp"



#ifdef X64
#error "Build in WIN32 (x86) configuration."
#endif



ReturnCode innerUpdate()
{
	Time::update();
	screen::update();

	HANDLE_ALL(singleton::instance->update());

	updateInput();

	return RC_SUCCESS;
}
void update() noexcept
{
	HANDLE_ALL(innerUpdate());

	glutPostRedisplay();
}



ReturnCode innerRender()
{
	HANDLE_ALL(singleton::instance->render());

	return RC_SUCCESS;
}
void render() noexcept
{
	glStencilMask(~0U);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	HANDLE_ALL(innerRender());

	glutSwapBuffers();
}



ReturnCode innerClose()
{
	if (ClosingState::Finished == g_closing) { return RC_SUCCESS; }

	g_closing = ClosingState::Closing;



	// application
	singleton::instance.destroyPtr(); // must be dot operator

	// audio
	destroyAudio();

	// error message
	g_reason.reset();

	// console thread
	console::destroy();
	


	g_closing = ClosingState::Finished;

	return RC_SUCCESS;
}
void close() noexcept
{
	try
	{
		ReturnCode const e = innerClose();
		if (e)
		{
			console::printCriticalError(stringError(e));
			std::terminate();
		}
	}
	catch (...)
	{
		console::printCriticalError(stringException());
		std::terminate();
	}
}



BOOL WINAPI onConsoleClose(DWORD ctrl)
{
	switch (ctrl)
	{
		case CTRL_CLOSE_EVENT:
		{
			if (ClosingState::Finished == g_closing)
			{
				break;
			}

			g_closing = ClosingState::WantingToClose;
			glutLeaveMainLoop();

			// wait until g_closing is Finished or the timer expires.
			constexpr long long maxWaitMilliseconds = 3'000;
			constexpr long long interval = 100;
			constexpr std::chrono::milliseconds ms{ interval };

			for (long long i = maxWaitMilliseconds / interval; i > 0; --i)
			{
				if (ClosingState::Finished == g_closing)
				{
					break;
				}
				std::this_thread::sleep_for(ms);
			}

			// should have finished closing by now.
			assert(ClosingState::Finished == g_closing);
			break;
		}
		default:
			break;
	}
	return FALSE;
}



ReturnCode innerMain(int argc, char** argv)
{
	// console thread
	console::init();

	// error message
	g_reason = std::make_unique<std::string>();

	// glut and glew
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen::getWidth(), screen::getHeight());
	glutCreateWindow("Rocket Chess");

	ASSERT0(glewInit());

	// initial GL properties
	glClearColor(1, 0, 0, 1);

	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// audio
	HANDLE_ALL(initAudio());

	// application
	HANDLE_ALL(singleton::instance.initPtr()); // must be dot operator

	// first update
	Time::init();
	update();

	// callbacks
	SetConsoleCtrlHandler(onConsoleClose, TRUE);

	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutCloseFunc(close);

	glutKeyboardFunc(onKeyboardDown);
	glutKeyboardUpFunc(onKeyboardUp);
	glutSpecialFunc(onKeyboardSpecialDown);
	glutSpecialUpFunc(onKeyboardSpecialUp);
	glutMouseFunc(onMouse);
	glutPassiveMotionFunc(onMouseMove);
	glutMotionFunc(onMouseDrag);
	
	glutMainLoop();
	close();
	return RC_SUCCESS;
}

int main(int argc, char** argv)
{
	try
	{
		auto const e = (innerMain(argc, argv));
		if (e)
		{
			console::printCriticalError(stringError(e));
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	catch (...)
	{
		console::printCriticalError(stringException());
		return EXIT_FAILURE;
	}
}
