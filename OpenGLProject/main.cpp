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
#include "application.hpp"
#include "singleton.hpp"

#include "time.hpp"
#include "screen.hpp"
#include "input.hpp"
#include "audio.hpp"

#include "closing_state.hpp"
#include "console.hpp"



#ifdef X64
#error "Build in WIN32 (x86) configuration."
#endif



void innerUpdate()
{
	Time::update();
	screen::update();

	try
	{
		singleton::instance->update();
	}
	catch (...) { printException(); }

	Input::update();
}
void update() noexcept
{
	try
	{
		innerUpdate();
	}
	catch (...) { printException(); }

	glutPostRedisplay();
}



void innerRender()
{
	singleton::instance->render();
}
void render() noexcept
{
	glStencilMask(~0U);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	try
	{
		innerRender();
	}
	catch (...) { printException(); }

	glutSwapBuffers();
}



void innerClose()
{
	if (ClosingState::Finished == g_closing) { return; }

	g_closing = ClosingState::Closing;



	// application
	singleton::instance.destroyPtr(); // must be dot operator

	// audio
	destroyAudio();

	// console thread
	console::destroy();
	


	g_closing = ClosingState::Finished;
}
void close() noexcept
{
	try
	{
		innerClose();
	}
	catch (...)
	{
		std::string const msg = stringException();
		console::printCriticalError(msg);
#ifdef _DEBUG
		assert(0);
#endif
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



void innerMain(int argc, char** argv)
{
	// console thread
	console::init();

	// glut and glew
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen::getWidth(), screen::getHeight());
	glutCreateWindow("Rocket Chess");

	if (glewInit()) { throw std::runtime_error("glewInit() failed."); }

	// initial GL properties
	glClearColor(0.0625, 0.0625, 0.0625, 1);

	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// time
	Time::init();

	// audio
	try
	{
		initAudio();
	}
	catch (...) { printException(); }

	// application
	try
	{
		singleton::instance.initPtr();
	}
	catch (...) { printException(); }

	// first update
	update();

	// callbacks
	SetConsoleCtrlHandler(onConsoleClose, TRUE);

	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutCloseFunc(close);

	glutKeyboardFunc(Input::onKeyboardDown);
	glutKeyboardUpFunc(Input::onKeyboardUp);
	glutSpecialFunc(Input::onKeyboardSpecialDown);
	glutSpecialUpFunc(Input::onKeyboardSpecialUp);
	glutMouseFunc(Input::onMouse);
	glutPassiveMotionFunc(Input::onMouseMove);
	glutMotionFunc(Input::onMouseDrag);
	
	glutMainLoop();
	close();
}

int main(int argc, char** argv)
{
	try
	{
		innerMain(argc, argv);
		return EXIT_SUCCESS;
	}
	catch (...)
	{
		std::string const msg = stringException();
		console::printCriticalError(msg);
#ifdef _DEBUG
		assert(0);
#endif
		return EXIT_FAILURE;
	}
}
