
#pragma once

#include "transform.hpp"
#include "text_renderer.hpp"



struct TextEntity
{
	Transform transform;
	TextRenderer textRenderer;
	TextMaterial material;

	TextEntity();

	void recalculate() { transform.recalculate(); }
	void render();
};
