/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	focused_panel.hpp
**
**	Summary		:	Variant cases used by Navigation class. Acts like a swift enum, holding extra data.
**
**	Project		:	GD2S02 Software Engineering
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	23/08/2019
*/

#pragma once

#include <variant>

#include "common.hpp"



// Each struct (e.g. FocusedPanel::MainMenu) is an enum case.
namespace FocusedPanel
{
	class MainMenu
	{
	public:
		enum class ButtonID
		{
			NewGame,
			Instructions,
			Options,
			ExitToDesktop,
			MAX = ExitToDesktop
		};
		ButtonID focusedButton{ ButtonID::NewGame };
	};

	class PawnPromotion {
	public:
		enum class ButtonID
		{
			Rook,
			Knight,
			Bishop,
			Queen,
			MAX = Queen
		};
		ButtonID focusedButton{};
	
	};

	class ChessBoard
	{
	private:
		ivec2 focusedCellCoords = ivec2(0, 0);
	public:
		ChessBoard() = default;
		explicit ChessBoard(ivec2 const& focusedCellCoords);

		ivec2 getFocusedCellCoords() const;
		void setFocusedCellCoords(ivec2 const& focusedCellCoords);
	};



	class RocketPurchase
	{
	public:
		enum class ButtonID
		{
			BackToBoard,
			RPG,
			ConventionalMissile,
			ICBM,
			Voyager1,
			MAX = Voyager1
		};
		ButtonID focusedButton{};
	};



	class EndTurn
	{
	public:
		enum class ButtonID
		{
			EndTurn,
			Undo,
			MAX = EndTurn
		};
		ButtonID focusedButton{};
	};



	class PauseMenu
	{
	public:
		enum class ButtonID
		{
			Continue,
			ExitToMainMenu,
			ExitToDesktop,
			MAX = ExitToDesktop
		};
		ButtonID focusedButton{};
	};
}



template<class...X> struct overload : X... { using X::operator()...; };
template<class...X> overload(X...)->overload<X...>;



void validateChessCoords(ivec2 const& coords);
