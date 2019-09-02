
#pragma once

#include "common.hpp"

enum class ChessActionType
{
	RegularMove,
	PawnPromotion,
	RocketAttack,
};

struct ChessAction
{
	ChessActionType type;
	ivec2 coords;

	explicit ChessAction(ChessActionType type, ivec2 coords) : type(type), coords(coords) {}
};
