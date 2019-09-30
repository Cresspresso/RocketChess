
#pragma once

#include "text_entity.hpp"

struct MultilineTextEntity
{
	TextEntity textEntity;

	glm::vec2 lineOffset = glm::vec2(0.0f, -16.0f);
	std::string text;

	void recalculate() { textEntity.recalculate(); }
	ReturnCode render();
};
