/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	skybox_entity.cpp
**
**	Summary		:	Entity for rendering a Skybox around the camera.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"
#include "world_math.hpp"

#include "textures.hpp"

#include "skybox_entity.hpp"

ReturnCode SkyboxEntity::render()
{
	renderer.modelMatrix = makeTransformMatrix(g_cameraEye, {}, vec3(scale));
	return renderer.render();
}
//
//SkyboxEntity::SkyboxEntity()
//{
//	renderer.mesh = &mesh;
//	renderer.material = &material;
//}
//
//ReturnCode SkyboxEntity::init()
//{
//	mesh = makeCubeMapMesh();
//	ReturnCode const r = loadCubeMap(&material.cubemap, "skybox", ".jpg", true);
//	return r;
//}
//
//void SkyboxEntity::destroy() noexcept
//{
//	destroyTexture(material.cubemap);
//	mesh.destroy();
//}
