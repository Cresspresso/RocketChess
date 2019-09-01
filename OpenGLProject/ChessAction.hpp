
#pragma once

#include "common.hpp"

enum class ChessActionType
{
	PawnMoveOne,
	PawnCapture,
	KingMove,
	PawnPromotion,
};

struct ChessAction
{
	ChessActionType type;
	ivec2 coords;

	explicit ChessAction(ChessActionType type, ivec2 coords) : type(type), coords(coords) {}
};
