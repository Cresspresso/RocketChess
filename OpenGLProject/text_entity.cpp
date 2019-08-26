
#include "resource_warehouse.hpp"
#include "singleton.hpp"

#include "text_entity.hpp"



TextEntity::TextEntity()
{
	auto& resources = singleton::getResources();

	textRenderer.font = &(resources.fonts[FontIndexer::Arial]);
	textRenderer.renderer.material = &material;
	textRenderer.renderer.program = resources.programs.getProgram(ProgramIndexer::Text);
	textRenderer.renderer.mesh = &(resources.meshes[MeshIndexer::Text]);
}



ReturnCode TextEntity::render()
{
	recalculate();
	textRenderer.renderer.modelMatrix = transform.modelMatrix;
	return textRenderer.render();
}
