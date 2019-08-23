
#include "focused_panel.hpp"

namespace FocusedPanel
{
	ChessBoard::ChessBoard(ivec2 const& focusedCellCoords) :
		focusedCellCoords(focusedCellCoords)
	{
		validateChessCoords(focusedCellCoords);
	}

	ivec2 ChessBoard::getFocusedCellCoords() const
	{
		return focusedCellCoords;
	}

	void ChessBoard::setFocusedCellCoords(ivec2 const& focusedCellCoords)
	{
		validateChessCoords(focusedCellCoords);
		this->focusedCellCoords = focusedCellCoords;
	}
}



void validateChessCoords(ivec2 const& coords)
{
	if (coords.x < 0 || coords.x >= 8
		|| coords.y < 0 || coords.y >= 8)
	{
		throw new std::out_of_range("cell coords out of range");
	}
}
