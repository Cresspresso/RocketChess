
#include "exceptions.hpp"

#include "multiline_text_entity.hpp"

void MultilineTextEntity::render()
{
	size_t current = 0;
	size_t next = text.find('\n');
	size_t i = 0;
	while (next != std::string::npos)
	{
		try {
			textEntity.textRenderer.text = text.substr(current, next - current);
			textEntity.textRenderer.position = static_cast<float>(i) * lineOffset;
			textEntity.render();
		}
		catch (...) { printException(); }

		current = next + 1;
		next = text.find('\n', current);
		++i;
	}

	try {
		textEntity.textRenderer.text = text.substr(current);
		textEntity.textRenderer.position = static_cast<float>(i) * lineOffset;
		textEntity.render();
	}
	catch (...) { printException(); }
}
