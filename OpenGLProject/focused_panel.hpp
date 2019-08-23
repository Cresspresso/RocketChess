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
**	Summary		:	Data class for which UI panel and button is focused by the player's cursor.
**
**	Project		:	GD2S02 Software Engineering
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	23/08/2019
*/

#pragma once

#include <variant>
#include <functional>

#include "common.hpp"



// The FocusedPanel class later in the file
// acts like a Swift enum, because it has extra data for each enum label.
// Each struct (e.g. FocusedPanels::MainMenuPanel) is an enum case.
namespace FocusedPanels
{
	struct MainMenuPanel
	{
		enum class ButtonID
		{
			NewGame,
			Instructions,
			Options,
			ExitToDesktop,
			MAX = ExitToDesktop
		};
		ButtonID focusedButton{ ButtonID::NewGame };

		MainMenuPanel() = default;

		inline explicit MainMenuPanel(ButtonID focusedButton)
			: focusedButton(focusedButton)
		{}
	};



	class ChessBoardPanel
	{
	private:
		ivec2 focusedCellCoords = ivec2(0, 0);
	public:
		ChessBoardPanel() = default;

		inline explicit ChessBoardPanel(ivec2 const& focusedCellCoords)
			: focusedCellCoords(focusedCellCoords)
		{
			if (focusedCellCoords.x < 0 || focusedCellCoords.x >= 8
				|| focusedCellCoords.y < 0 || focusedCellCoords.y >= 8)
			{
				throw new std::out_of_range("cell coords out of range");
			}
		}

		inline void setFocusedCellCoords(ivec2 const& focusedCellCoords)
		{
			if (focusedCellCoords.x < 0 || focusedCellCoords.x >= 8
				|| focusedCellCoords.y < 0 || focusedCellCoords.y >= 8)
			{
				throw new std::out_of_range("cell coords out of range");
			}
			this->focusedCellCoords = focusedCellCoords;
		}
		inline ivec2 getFocusedCellCoords() const { return focusedCellCoords; }
	};



	struct RocketPurchasePanel
	{
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

		RocketPurchasePanel() = default;

		inline explicit RocketPurchasePanel(ButtonID focusedButton)
			: focusedButton(focusedButton)
		{}
	};



	struct EndTurnPanel
	{
		enum class ButtonID
		{
			EndTurn,
			Undo,
			MAX = EndTurn
		};
		ButtonID focusedButton{};

		EndTurnPanel() = default;

		inline explicit EndTurnPanel(ButtonID focusedButton)
			: focusedButton(focusedButton)
		{}
	};



	struct PauseMenuPanel
	{
		enum class ButtonID
		{
			Continue,
			ExitToMainMenu,
			ExitToDesktop,
			MAX = ExitToDesktop
		};
		ButtonID focusedButton{};

		PauseMenuPanel() = default;

		inline explicit PauseMenuPanel(ButtonID focusedButton)
			: focusedButton(focusedButton)
		{}
	};
}



class FocusedPanel
{
private:

	std::variant<
		FocusedPanels::MainMenuPanel,
		FocusedPanels::ChessBoardPanel,
		FocusedPanels::RocketPurchasePanel,
		FocusedPanels::EndTurnPanel,
		FocusedPanels::PauseMenuPanel
	> variant;


public:

	// Sets the enum case.
	// For example:
	//   fp.emplace<FocusedPanels::MainMenuPanel>();
	template<class T, class...Args>
	T& emplace(Args&&...args)
	{
		return variant.emplace<T>(std::forward<Args>(args)...);
	}



	template<class F>
	auto visit(F visitor)
	{
		return std::visit(visitor, variant);
	}



	// Does a switch statement like operation on this variant.
	// Each argument is a callback:
	//   First argument: the appropriate Navigation Panel type by reference.
	//   Returns: value of type R.
	// This function returns the return value of the callback that was invoked.
	template<class R>
	R visit(
		std::function<R(FocusedPanels::MainMenuPanel&)> visitMainMenuPanel,
		std::function<R(FocusedPanels::ChessBoardPanel&)> visitChessBoardPanel,
		std::function<R(FocusedPanels::RocketPurchasePanel&)> visitRocketPurchasePanel,
		std::function<R(FocusedPanels::EndTurnPanel&)> visitEndTurnPanel,
		std::function<R(FocusedPanels::PauseMenuPanel&)> visitPauseMenuPanel
		)
	{
		using namespace FocusedPanels;

		struct Visitor
		{
			decltype(visitMainMenuPanel) visitMainMenuPanel;
			decltype(visitChessBoardPanel) visitChessBoardPanel;
			decltype(visitRocketPurchasePanel) visitRocketPurchasePanel;
			decltype(visitEndTurnPanel) visitEndTurnPanel;
			decltype(visitPauseMenuPanel) visitPauseMenuPanel;

			R operator()(MainMenuPanel& a) { return visitMainMenuPanel(a); }
			R operator()(ChessBoardPanel& a) { return visitChessBoardPanel(a); }
			R operator()(RocketPurchasePanel& a) { return visitRocketPurchasePanel(a); }
			R operator()(EndTurnPanel& a) { return visitEndTurnPanel(a); }
			R operator()(PauseMenuPanel& a) { return visitPauseMenuPanel(a); }
		};

		Visitor visitor{
			visitMainMenuPanel,
			visitChessBoardPanel,
			visitRocketPurchasePanel,
			visitEndTurnPanel,
			visitPauseMenuPanel
		};

		return std::visit(visitor, variant);
	}
};
