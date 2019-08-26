
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



void MainMenuButtons::highlight(std::optional<size_t> index)
{
	for (auto& button : buttons)
	{
		button.buttonEntity.transform.localScale = vec3(1);
	}

	if (index)
	{
		buttons[*index].buttonEntity.transform.localScale = vec3(1.2f, 1.2f, 1);
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
