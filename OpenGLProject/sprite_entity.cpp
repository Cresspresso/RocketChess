
#include "resource_warehouse.hpp"
#include "singleton.hpp"

#include "sprite_entity.hpp"



SpriteEntity::SpriteEntity(TextureIndexer id)
{
	auto& resources = singleton::getResources();

	renderer.material = &material;
	renderer.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	renderer.mesh = &(resources.meshes[MeshIndexer::Quad]);

	setTexture(id);

	transform.localScale = vec3(100, 100, 1);
}



void SpriteEntity::setTexture(TextureIndexer id)
{
	material.tex1 = singleton::getResources().textures[id];
}



void SpriteEntity::render()
{
	transform.recalculate();
	renderer.modelMatrix = transform.modelMatrix;
	return renderer.render();
}
