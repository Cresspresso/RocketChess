
#include "application.hpp"
#include "singleton.hpp"

#include "main_menu_button_renderer.hpp"



MainMenuButtonRenderer::MainMenuButtonRenderer()
{
	auto& resources = singleton::getResources();

	matBG.tex1 = resources.textures[TextureIndexer::Button];

	br.background.material = &matBG;
	br.background.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	br.background.mesh = &(resources.meshes[MeshIndexer::Quad]);

	br.backgroundTransform.localScale = vec3(120,60,1);

	br.text.font = &(resources.fonts[FontIndexer::Arial]);

	br.textTransform.localPosition = vec3(-br.backgroundTransform.localScale.x, 0, 0);
}

ReturnCode MainMenuButtonRenderer::render()
{
	return br.render();
}
