/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	basic_materials_builder.cpp
**
**	Summary		:	A materials builder initialises a collection of materials used in a scene.
**					The basic materials are:
**						Skybox
**						Menu Button Background
**						Menu Button Text
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "resource_warehouse.hpp"
#include "singleton.hpp"

#include "basic_materials_builder.hpp"



BasicMaterialsBuilder::BasicMaterialsBuilder() :
	resources{ singleton::getResources() }
{}



void BasicMaterialsBuilder::initSkybox(SkyboxMaterial& material) const
{
	material.cubemap = resources.cubemaps[CubeMapIndexer::Skybox];
}

void BasicMaterialsBuilder::initButtonBackground(Quad4Material& material) const
{
	material.tex1 = resources.textures[TextureIndexer::Button];
}

void BasicMaterialsBuilder::initButtonText(TextMaterial& material) const
{
	material.tint = vec3(0);
}



void BasicMaterialsBuilder::build(BasicMaterials& materials) const
{
	initSkybox(materials.skybox);
	initButtonBackground(materials.buttonBackground);
	initButtonText(materials.buttonText);
}
