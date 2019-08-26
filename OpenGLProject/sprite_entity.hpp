
#pragma once

#include "transform.hpp"
#include "renderer.hpp"
#include "quad4_material.hpp"
#include "texture_manager.hpp"



struct SpriteEntity
{
	Transform transform;
	Renderer renderer;
	Quad4Material material;

	SpriteEntity();

	void setTexture(TextureIndexer id);

	void recalculate() { transform.recalculate(); }
	ReturnCode render();
};
