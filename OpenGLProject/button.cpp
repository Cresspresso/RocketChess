/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_button.cpp
**
**	Summary		:	Button that executes an action when it is clicked or a hotkey is pressed.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <array>
#include <glm/gtx/intersect.hpp>

#include "input.hpp"
#include "globals.hpp"

#include "button.hpp"



Button::Button()
{
	auto* parent = &transform.modelMatrix;

	colliderTransform.parentModelMatrix = parent;
	backgroundTransform.parentModelMatrix = parent;
	textTransform.parentModelMatrix = parent;
}



void Button::recalculate()
{
	transform.recalculate();
	colliderTransform.recalculate();
	backgroundTransform.recalculate();
	textTransform.recalculate();
}



ReturnCode Button::update()
{
	BEGIN_ANYALL();

	DO_ANYALL(onClickLeft.update());
	DO_ANYALL(onClickRight.update());
	updatePointerCheck();

	return END_ANYALL();
}



ReturnCode Button::render()
{
	BEGIN_ANYALL();

	background.modelMatrix = backgroundTransform.modelMatrix;
	DO_ANYALL(background.render());

	text.renderer.modelMatrix = textTransform.modelMatrix;
	DO_ANYALL(text.render());

	return END_ANYALL();
}



//namespace
//{
//	struct Plane
//	{
//		vec3 point;
//		vec3 normal;
//	};
//
//	// TODO test this (normals may be wrong)
//	std::array<Plane, 6> calculateColliderPlanes(mat4 const& modelMatrix)
//	{
//		static std::array<vec3, 6> const localNormals
//		{
//			vec3(0, 0, 1),
//			vec3(0, 0, -1),
//			vec3(0, 1, 0),
//			vec3(0, -1, 0),
//			vec3(1, 0, 0),
//			vec3(-1, 0, 0),
//		};
//
//		mat3 const fixNormals = calculateFixNormalsMatrix(modelMatrix);
//
//		std::array<Plane, 6> planes;
//		for (size_t i = 0; i < 6; ++i)
//		{
//			planes[i].point = transformPoint(modelMatrix, localNormals[i]);
//			planes[i].normal = glm::normalize(fixNormals * localNormals[i]);
//		}
//		return planes;
//	}
//}



bool Button::isMouseInsideQuad()
{
	// get camera properties
	mat4 const& vpMatrix = g_cameraVPMatrix;
	vec3 const& eye = g_cameraEye;

	// get mouse ray in world space
	vec3 const rayVector = 10'000.f * makeRayDirection(getMouseViewPos(), vpMatrix);

	// get quad points in world space...
	mat4 const& colliderModelMatrix = colliderTransform.modelMatrix;
	//mat3 const fixNormals = calculateFixNormalsMatrix(modelMatrix);
	//vec3 const planeNormal = fixNormals * vec3(0, 0, 1); // normal faces outwards from button quad
	//vec3 const planePoint = transformPoint(modelMatrix, vec3(0, 0, 0)); // plane is on face of button quad

	// generate points of the quad
	vec3 points[4] =
	{
		vec3(-1, -1, 0),
		vec3(1, -1, 0),
		vec3(1, 1, 0),
		vec3(-1, 1, 0),
	};

	// apply collider transformation to the points
	for (vec3& point : points)
	{
		point = transformPoint(colliderModelMatrix, point);
	}

	// check if mouse ray intersects the quad
	vec3 baryPos;
	bool const a = glm::intersectRayTriangle(eye, rayVector, points[0], points[1], points[2], baryPos);
	bool const b = glm::intersectRayTriangle(eye, rayVector, points[2], points[3], points[0], baryPos);

	bool const isMouseInsideQuad = a || b;
	return isMouseInsideQuad;
}



void Button::updatePointerCheck()
{
	bool const left = InputState::UpFirst == getMouseButtonState(MOUSE_LEFT);
	bool const right = InputState::UpFirst == getMouseButtonState(MOUSE_RIGHT);

	// if user is not clicking a mouse button...
	if (!left && !right)
	{
		return;
	}

	// if mouse ray does not intersect quad...
	if (!isMouseInsideQuad())
	{
		return;
	}

	// invoke the button clicked event.
	if (left)
	{
		HANDLE_ALL(onClickLeft());
	}
	if (right)
	{
		HANDLE_ALL(onClickRight());
	}
}



ReturnCode Button::ClickEvent::update()
{
	if (hotkeys.empty()) { return RC_SUCCESS; }

	auto const pred = [](unsigned char key)
	{
		return InputState::DownFirst == getKeyboardState(key);
	};
	bool const triggered = std::any_of(hotkeys.begin(), hotkeys.end(), pred);

	if (triggered)
	{
		// a hotkey was pressed.
		// invoke the action.
		return operator()();
	}
	else
	{
		// no hotkey was pressed.
		return RC_SUCCESS;
	}
}



ReturnCode Button::ClickEvent::operator()()
{
	if (!action) { return RC_SUCCESS; }
	return action();
}
