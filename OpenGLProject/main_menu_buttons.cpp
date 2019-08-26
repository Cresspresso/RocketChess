
#include "main_menu_buttons.hpp"



MainMenuButtons::MainMenuButtons(vec3 const & position, vec3 const & offset, std::vector<std::string> const & texts) :
	buttons(texts.size())
{
	for (size_t i = 0; i < texts.size(); i++)
	{
		auto& button = buttons[i];
		button.buttonEntity.transform.localPosition = position + offset * static_cast<float>(i);
		button.buttonEntity.textEntity.textRenderer.text = texts[i];
	}
}



ReturnCode MainMenuButtons::render()
{
	BEGIN_ANYALL();
	for (auto& button : buttons)
	{
		DO_ANYALL(button.render());
	}
	return END_ANYALL();
}
