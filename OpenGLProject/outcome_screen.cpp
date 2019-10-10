
#include "outcome_screen.hpp"

OutcomeScreen::OutcomeScreen()
	: m_buttons(
		glm::vec3(-300, -300, 0),
		glm::vec3(),
		{ std::string("Done") })
{
	m_buttons.highlight(0);

	m_title.transform.localPosition = vec3(0, 300, 0);
	m_title.textRenderer.text = "Victory";

	m_winnerNation.transform.localPosition = vec3(0, 260, 0);
	m_winnerNation.textRenderer.text = "USSR";

	m_methodOfVictory.textEntity.transform.localPosition = vec3(-400, -200, 0);
	m_methodOfVictory.text = R"__(By launching the rocket you have reached space.)__";

	m_winnerFlag.transform.localPosition = vec3(100, 100, 0);
	m_winnerFlag.transform.localScale = vec3(600, 300, 1);
}

void OutcomeScreen::UpdateWinner(bool winnerIsUS, bool victoryByVoyager)
{
	std::string const winner = winnerIsUS ? "US" : "USSR";
	std::string const loser = winnerIsUS ? "USSR" : "US";

	m_winnerNation.textRenderer.text = winner;

	m_methodOfVictory.text = victoryByVoyager
		? "By launching the Voyager 1 Rocket, the " + winner + " has won\nthe space race. The cold war is wrapping up."
		: "By 'communicating' with the " + loser + " leadership,\nthe " + winner + " has started wrapping up the cold war.";

	m_winnerFlag.setTexture(winnerIsUS ? TextureIndexer::USFLAG : TextureIndexer::USSRFLAG);
}

void OutcomeScreen::render()
{
	m_winnerFlag.render();
	m_buttons.render();
	m_title.render();
	m_winnerNation.render();
	m_methodOfVictory.render();
}
