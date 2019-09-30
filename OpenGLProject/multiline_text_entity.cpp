
#include "multiline_text_entity.hpp"

ReturnCode MultilineTextEntity::render()
{
	BEGIN_ANYALL();
	size_t current = 0;
	size_t next = text.find('\n');
	size_t i = 0;
	while (next != std::string::npos)
	{
		textEntity.textRenderer.text = text.substr(current, next - current);
		textEntity.textRenderer.position = static_cast<float>(i) * lineOffset;
		DO_ANYALL(textEntity.render());

		current = next + 1;
		next = text.find('\n', current);
		++i;
	}

	textEntity.textRenderer.text = text.substr(current);
	textEntity.textRenderer.position = static_cast<float>(i) * lineOffset;
	DO_ANYALL(textEntity.render());

	return END_ANYALL();
}
