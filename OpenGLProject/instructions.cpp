
#include "instructions.hpp"

Instructions::Instructions()
	: m_buttons(
		glm::vec3(-300, -300, 0),
		glm::vec3(),
		{ std::string("Back") })
{
	m_buttons.highlight(0);

	m_title.transform.localPosition = vec3(0, 300, 0);
	m_title.textRenderer.text = "Instructions";

	m_instructions.textEntity.transform.localPosition = vec3(-100, 0, 0);
	m_instructions.text = R"__(Move your cursor over a piece.
Press SPACE to show available actions.
Press SPACE on a blue cell to move
your piece there. Then it's the next
player's turn. When you capture an enemy
piece, you will be rewarded money.
You can buy rockets in the TAB menu.
Higher level rockets can target higher
value pieces. The first player to
capture the enemy King or buy the
Voyager 1 Rocket wins the game.)__";

	m_controls.textEntity.transform.localPosition = vec3(-500, 0, 0);
	m_controls.text = R"__(W/S/A/D to move cursor.
SPACE to press button.
ESC to toggle pause menu.
TAB to toggle rocket menu.)__";
}

ReturnCode Instructions::render()
{
	BEGIN_ANYALL();
	DO_ANYALL(m_buttons.render());
	DO_ANYALL(m_title.render());
	DO_ANYALL(m_instructions.render());
	DO_ANYALL(m_controls.render());
	return END_ANYALL();
}
