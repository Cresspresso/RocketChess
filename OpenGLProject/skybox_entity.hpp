/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	skybox_entity.hpp
**
**	Summary		:	Entity for rendering a Skybox around the camera.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "renderer.hpp"
//#include "cubemap_material.hpp"
//#include "cubemap_mesh.hpp"

struct SkyboxEntity
{
	void render();
	//SkyboxEntity();
	//void init();
	//void destroy() noexcept;



	/*SkyboxMaterial material;
	Mesh mesh;*/
	Renderer renderer;
	float scale = 2000;
};
