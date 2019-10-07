
#include "credits.hpp"

Credits::Credits()
	: m_buttons(
		glm::vec3(-300, -300, 0),
		glm::vec3(),
		{ std::string("Back") })
{
	m_buttons.highlight(0);

	m_title.transform.localPosition = vec3(0, 300, 0);
	m_title.textRenderer.text = "Credits";

	m_credits.textEntity.transform.localPosition = vec3(-100, 0, 0);
	m_credits.text = R"__(Ryan Wilkinson
Elijah Shadbolt
Harrison Orsbourne)__";
}

ReturnCode Credits::render()
{
	BEGIN_ANYALL();
	DO_ANYALL(m_buttons.render());
	DO_ANYALL(m_title.render());
	DO_ANYALL(m_credits.render());
	return END_ANYALL();
}
