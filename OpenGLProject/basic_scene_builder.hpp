/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	basic_scene_builder.hpp
**
**	Summary		:	A scene builder initialises a collection of entities in the scene.
**					The basic scene entities are:
**						World Camera
**						Skybox
**						HUD Camera
**						Menu Button
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "camera_orbit_entity.hpp"
#include "skybox_entity.hpp"
#include "menu_button.hpp"

#include "font.hpp"
#include "cubemap_material.hpp"

#include "resource_warehouse.hpp"

#include "basic_materials_builder.hpp"



struct BasicSceneEntities
{
	CameraOrbitEntity cameraWorld;
	SkyboxEntity skybox;

	Camera cameraHud;
	MenuButtonEntity buttonMenu;
};



struct BasicSceneBuilder
{
	ResourceWarehouse& resources;
	SkyboxMaterial* materialSkybox = nullptr;
	TextMaterial* materialText = nullptr;
	IMaterial* materialButtonBackground = nullptr;

	BasicSceneBuilder();
	BasicSceneBuilder(BasicMaterials& materials);

	// world
	void initWorldCamera(CameraOrbitEntity& entity) const;
	void initSkybox(SkyboxEntity& entity) const;

	// hud
	void initHudCamera(Camera& entity) const;
	void initTextRenderer(
		TextRenderer& entity,
		TextMaterial* material = nullptr,
		Font* font = nullptr
	) const;
	void initButton(Button& entity) const;
	void initMenuButton(MenuButtonEntity& entity) const;

	void build(BasicSceneEntities& entities) const;



	BasicSceneBuilder(BasicSceneBuilder const&) = delete;
	BasicSceneBuilder(BasicSceneBuilder&&) = delete;
	BasicSceneBuilder& operator=(BasicSceneBuilder const&) = delete;
	BasicSceneBuilder& operator=(BasicSceneBuilder&&) = delete;
	~BasicSceneBuilder() = default;
};
