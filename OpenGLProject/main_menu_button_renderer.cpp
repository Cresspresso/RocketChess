
#include "main_menu_button_renderer.hpp"



MainMenuButtonRenderer::MainMenuButtonRenderer()
{
	br.backgroundTransform.localScale = vec3(200,80,1);
	br.textTransform.localPosition = vec3(-br.backgroundTransform.localScale.x / 2 + 10, 0, 0);
	br.matText.tint = vec3(0);
}

ReturnCode MainMenuButtonRenderer::render()
{
	return br.render();
}
