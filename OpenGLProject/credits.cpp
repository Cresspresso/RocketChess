
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

	m_credits.textEntity.transform.localPosition = vec3(-100, 200, 0);
	m_credits.text = R"__(Programming & Design
Ryan Wilkinson
Elijah Shadbolt
Harrison Orsbourne

Music & Sound
COAG Music
)__";
}

void Credits::render()
{
	try {
		m_buttons.render();
	}
	catch (...) { printException(); }

	try {
		m_title.render();
	}
	catch (...) { printException(); }

	try {
		m_credits.render();
	}
	catch (...) { printException(); }
}
