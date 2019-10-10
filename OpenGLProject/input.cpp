/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	input.cpp
**
**	Summary		:	Mouse and keyboard input.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <array>

#include "common.hpp"
#include "screen.hpp"
#include "world_math.hpp" // for screenToWorldPoint
#include "exceptions.hpp"

#include "input.hpp"



namespace Input
{
	namespace
	{
		static std::array<InputState, 255> g_keyboardStates{};
		static std::array<InputState, 255> g_specialStates{};
		static std::array<InputState, 10> g_mouseButtonStates{};


		static ivec2 g_mousePos = ivec2();
		static ivec2 g_mouseLastPos = ivec2();
		static ivec2 g_mouseDeltaPos = ivec2();
		static vec2 g_mouseViewPos = vec2(-1, 1);



		vec2 calculateMouseViewPos(vec2 const& mousePos) noexcept
		{
			vec2 const screenHalfSize = screen::getHalfSizeF();
			vec2 viewPos = screenPointToViewPoint(mousePos, screenHalfSize);
			viewPos.y = -viewPos.y;
			return viewPos;
		}



		static void setMousePos(int x, int y)
		{
			g_mousePos = ivec2(x, y);
			g_mouseViewPos = calculateMouseViewPos(vec2(x, y));
		}



		void innerKeyboardDown(unsigned char key, int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);

			auto& states = g_keyboardStates;
			if (key < 0 || key >= states.size())
			{
				throw std::runtime_error("Keyboard key out of range: " + std::to_string(key));
			}

			InputState& state = states[key];
			if (state == InputState::Down)
			{
				state = InputState::DownAgain;
			}
			else
			{
				state = InputState::DownFirst;
			}
		}

		void innerKeyboardUp(unsigned char key, int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);

			auto& states = g_keyboardStates;
			if (key < 0 || key >= states.size())
			{
				throw std::runtime_error("Keyboard key out of range: " + std::to_string(key));
			}

			InputState& state = states[key];
			if (state == InputState::Up)
			{
				state = InputState::UpAgain;
			}
			else
			{
				state = InputState::UpFirst;
			}
		}



		/*void clearKeyboardInput()
		{
			auto& states = g_keyboardStates;
			for (InputState& state : states)
			{
				state = InputState::Up;
			}
		}*/

		void innerKeyboardSpecialDown(int key, int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);

			auto& states = g_specialStates;
			if (key < 0 || static_cast<size_t>(key) >= states.size())
			{
				throw std::runtime_error("Special key out of range: " + std::to_string(key));
			}

			InputState& state = states[key];
			if (state == InputState::Down)
			{
				state = InputState::DownAgain;
			}
			else
			{
				state = InputState::DownFirst;
			}
		}

		void innerKeyboardSpecialUp(int key, int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);

			auto& states = g_specialStates;
			if (key < 0 || static_cast<size_t>(key) >= states.size())
			{
				throw std::runtime_error("Special key out of range: " + std::to_string(key));
			}

			InputState& state = states[key];
			if (state == InputState::Up)
			{
				state = InputState::UpAgain;
			}
			else
			{
				state = InputState::UpFirst;
			}
		}

		void innerMouse(int button, int state, int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);

			auto& states = g_mouseButtonStates;
			if (button < 0 || static_cast<size_t>(button) >= states.size())
			{
				throw std::runtime_error("Mouse button out of range: " + std::to_string(button));
			}

			auto& st = states[button];
			if (state == GLUT_DOWN)
			{
				if (st == InputState::Down)
				{
					st = InputState::DownAgain;
				}
				else
				{
					st = InputState::DownFirst;
				}
			}
			else
			{
				st = InputState::UpFirst;
			}
		}

		void innerMouseMove(int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);
		}

		void innerMouseDrag(int mouseX, int mouseY)
		{
			setMousePos(mouseX, mouseY);
		}

		template<size_t N>
		void updateInputStates(std::array<InputState, N>& map)
		{
			for (InputState& state : map)
			{
				switch (state)
				{
				case InputState::DownAgain:
				case InputState::DownFirst:
					state = InputState::Down;
					break;

				case InputState::UpFirst:
				case InputState::UpAgain:
					state = InputState::Up;
					break;

				case InputState::Down:
				case InputState::Up:
				default:
					break;
				}
			}
		}
	}



	bool isInputStateDown(InputState state) noexcept
	{
		switch (state)
		{
		case InputState::Down:
		case InputState::DownFirst:
		case InputState::DownAgain:
			return true;

		case InputState::Up:
		case InputState::UpFirst:
		case InputState::UpAgain:
		default:
			return false;
		}
	}



	void onKeyboardDown(unsigned char key, int mouseX, int mouseY) noexcept
	{
		try {
			innerKeyboardDown(key, mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onKeyboardUp(unsigned char key, int mouseX, int mouseY) noexcept
	{
		try {
			innerKeyboardUp(key, mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onKeyboardSpecialDown(int key, int mouseX, int mouseY) noexcept
	{
		try {
			innerKeyboardSpecialDown(key, mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onKeyboardSpecialUp(int key, int mouseX, int mouseY) noexcept
	{
		try {
			innerKeyboardSpecialUp(key, mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onMouse(int button, int state, int mouseX, int mouseY) noexcept
	{
		try {
			innerMouse(button, state, mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onMouseMove(int mouseX, int mouseY) noexcept
	{
		try {
			innerMouseMove(mouseX, mouseY);
		}
		catch (...) { printException(); }
	}

	void onMouseDrag(int mouseX, int mouseY) noexcept
	{
		try {
			innerMouseDrag(mouseX, mouseY);
		}
		catch (...) { printException(); }
	}



	ivec2 const& getMousePos() noexcept
	{
		return g_mousePos;
	}
	ivec2 const& getMouseDeltaPos() noexcept
	{
		return g_mouseDeltaPos;
	}
	vec2 const& getMouseViewPos() noexcept
	{
		return g_mouseViewPos;
	}

	InputState getKeyboardState(unsigned char key)
	{
		auto& states = g_keyboardStates;
		if (key < 0 || static_cast<size_t>(key) >= states.size())
		{
			throw std::runtime_error("Keyboard key out of range: " + std::to_string(key));
		}
		return states[key];
	}

	InputState getSpecialState(int key)
	{
		auto& states = g_specialStates;
		if (key < 0 || static_cast<size_t>(key) >= states.size())
		{
			throw std::runtime_error("Special key out of range: " + std::to_string(key));
		}
		return states[key];
	}

	InputState getMouseButtonState(int button)
	{
		auto& states = g_mouseButtonStates;
		if (button < 0 || static_cast<size_t>(button) >= states.size())
		{
			throw std::runtime_error("Mouse button out of range: " + std::to_string(button));
		}
		return states[button];
	}



	bool isKeyboardKeyDown(unsigned char key)
	{
		return isInputStateDown(getKeyboardState(key));
	}

	bool isSpecialKeyDown(int key)
	{
		return isInputStateDown(getSpecialState(key));
	}

	bool isMouseButtonDown(int button)
	{
		return isInputStateDown(getMouseButtonState(button));
	}



	void update()
	{
		updateInputStates(g_keyboardStates);
		updateInputStates(g_specialStates);
		updateInputStates(g_mouseButtonStates);

		g_mouseDeltaPos = g_mousePos - g_mouseLastPos;
		g_mouseLastPos = g_mousePos;
	}
}
