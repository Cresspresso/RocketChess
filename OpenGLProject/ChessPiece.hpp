
#pragma once

#include <ostream>

enum class ChessPiece
{
	None,
	Pawn,
	King,
	Queen,
	Knight,
	Bishop,
	Rook,
};

inline char const* to_str(ChessPiece value)
{
	switch (value)
	{
	case ChessPiece::Bishop:
		return "Bishop";
	case ChessPiece::King:
		return "King";
	case ChessPiece::Knight:
		return "Knight";
	case ChessPiece::Pawn:
		return "Pawn";
	case ChessPiece::Queen:
		return "Queen";
	case ChessPiece::Rook:
		return "Rook";
	case ChessPiece::None:
		return "None";
	default:
		break;
	}
	return "ERROR";
}

inline std::ostream& operator<<(std::ostream& outs, ChessPiece value)
{
	return outs << to_str(value);
}

inline char const* symbol(ChessPiece value)
{
	switch (value)
	{
	case ChessPiece::Bishop:
		return "B";
	case ChessPiece::King:
		return "K";
	case ChessPiece::Knight:
		return "N";
	case ChessPiece::Pawn:
		return "P";
	case ChessPiece::Queen:
		return "Q";
	case ChessPiece::Rook:
		return "R";
	case ChessPiece::None:
		return "-";
	default:
		break;
	}
	return "?";
}
