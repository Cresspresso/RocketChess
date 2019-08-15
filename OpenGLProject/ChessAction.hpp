
#pragma once

#include "common.hpp"

enum class ChessActionType
{
	PawnMoveOne,
	PawnCapture,
	KingMove,
};

struct ChessAction
{
	ChessActionType type;
	ivec2 coords;
};
