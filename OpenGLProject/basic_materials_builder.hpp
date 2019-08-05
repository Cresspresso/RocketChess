/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	basic_materials_builder.hpp
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

#pragma once

#include "cubemap_material.hpp"
#include "quad4_material.hpp"
#include "font.hpp"

#include "resource_warehouse.hpp"



struct BasicMaterials
{
	SkyboxMaterial skybox;
	Quad4Material buttonBackground;
	TextMaterial buttonText;
};



struct BasicMaterialsBuilder
{
	ResourceWarehouse& resources;

	BasicMaterialsBuilder();

	void initSkybox(SkyboxMaterial& material) const;
	void initButtonBackground(Quad4Material& material) const;
	void initButtonText(TextMaterial& material) const;

	void build(BasicMaterials& materials) const;



	BasicMaterialsBuilder(BasicMaterialsBuilder const&) = delete;
	BasicMaterialsBuilder(BasicMaterialsBuilder&&) = delete;
	BasicMaterialsBuilder& operator=(BasicMaterialsBuilder const&) = delete;
	BasicMaterialsBuilder& operator=(BasicMaterialsBuilder&&) = delete;
	~BasicMaterialsBuilder() = default;
};
