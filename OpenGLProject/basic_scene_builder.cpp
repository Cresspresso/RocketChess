/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	basic_scene_builder.cpp
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

#include "singleton.hpp"
#include "input.hpp"

#include "basic_scene_builder.hpp"



BasicSceneBuilder::BasicSceneBuilder() :
	resources{ singleton::getResources() }
{}

BasicSceneBuilder::BasicSceneBuilder(BasicMaterials& materials) :
	BasicSceneBuilder()
{
	materialSkybox = &(materials.skybox);
	materialText = &(materials.buttonText);
	materialButtonBackground = &(materials.buttonBackground);
}



void BasicSceneBuilder::initWorldCamera(CameraOrbitEntity& entity) const
{
	entity.camera.projection.isOrthographic = false;

	entity.control.minRadius = 10;
	entity.control.maxRadius = 20;
	entity.control.eulerAngles = vec3(-60, 0, 0);

	entity.recalculate();
}



void BasicSceneBuilder::initSkybox(SkyboxEntity& entity) const
{
	auto& r = entity.renderer;
	r.program = resources.programs[ProgramIndexer::Skybox].program;
	r.mesh = &(resources.meshes[MeshIndexer::Skybox]);
	r.material = materialSkybox; // &(materials.skybox);
}



void BasicSceneBuilder::initHudCamera(Camera& camera) const
{
	auto& proj = camera.projection;
	proj.isOrthographic = true;
	proj.halfHeight = 400;

	camera.transform.eye = vec3(0, 0, proj.nearClip + (0.5f * proj.farClip));

	camera.recalculate();
}



void BasicSceneBuilder::initTextRenderer(
	TextRenderer & entity,
	TextMaterial * material,
	Font * font
) const
{
	entity.font = font ? font : &(resources.fonts[FontIndexer::Arial]);

	// Note: Mesh vertex buffer is modified at runtime.
	// DO NOT RENDER IN MULTITHREAD.
	auto& r = entity.renderer;
	r.program = resources.programs[ProgramIndexer::Text].program;
	r.mesh = &(resources.meshes[MeshIndexer::Text]);
	r.material = material ? material : materialText;
}



void BasicSceneBuilder::initButton(Button & button) const
{
	// init collider transform
	{
		auto& ct = button.colliderTransform;
		ct.localScale = vec3(120, 30, 1);
	}

	// init background transform
	{
		auto& bgt = button.backgroundTransform;
		bgt.localScale = vec3(2.f * vec2(button.colliderTransform.localScale), 1);
	}

	// init background
	{
		auto& bg = button.background;
		bg.program = resources.programs[ProgramIndexer::Quad4].program;
		bg.mesh = &(resources.meshes[MeshIndexer::Quad]);
		bg.material = materialButtonBackground;// &(materials.buttonMenuBackground);
	}

	// init text renderer
	{
		auto& tx = button.text;
		initTextRenderer(tx); // &(materials.buttonMenuText);
		tx.scale = vec2(1);

		vec3 const buttonHalfSize = 0.5f * button.backgroundTransform.localScale;
		tx.position = vec2(-buttonHalfSize.x, -4);
	}
}



void BasicSceneBuilder::initMenuButton(MenuButtonEntity& entity) const
{
	auto& button = entity.button;
	initButton(button);

	// set callback
	button.onClickLeft.hotkeys.push_back(KEY_ESCAPE);
	button.onClickLeft.action = []
	{
		singleton::postLoadSceneEvent(SceneIndexer::Menu);
		return RC_SUCCESS;
	};

	// init text renderer
	{
		auto& tx = button.text;
		tx.text = "Menu (ESC)";
		tx.position.x += 20;
	}
}



void BasicSceneBuilder::build(BasicSceneEntities& entities) const
{
	initWorldCamera(entities.cameraWorld);
	initSkybox(entities.skybox);

	initHudCamera(entities.cameraHud);
	initMenuButton(entities.buttonMenu);
}
