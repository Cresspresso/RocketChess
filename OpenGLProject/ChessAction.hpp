
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
	int cost;

	explicit ChessAction(ChessActionType type, ivec2 coords, int cost = 0) : type(type), coords(coords), cost(cost) {}
};
