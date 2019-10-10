
#include "MissileManager.h"
#include "ChessPiece.hpp"
#include "menu_scene.hpp"

#include "console.hpp"

//checks to see which missile the player has tried to purchase
//missileChoice is which missile they want to buy 
void missileManager::purchaseMissile(int missileChoice){

	//switch case for each missile type
	switch (missileChoice) {
	//TODO: check to see whose turn it is
	
	//rpg missile
	case 1:
		launchableMissile = RPG;
		//move to choosing missile launch position
		launchMissile();
		break;
	//ballistic missile
	case 2:
		launchableMissile = BALLISTIC;
		launchMissile();
		break;
	//ICBM missile
	case 3:
		launchableMissile = ICBM;
		launchMissile();
		break;
	//launch the voyager
	case 4:
		launchableMissile = VOYAGER;
		//TODO: Make it so the voyager just makes the player who launched it win the game
		launchMissile();
		break;

	//default case
	default:
		break;
	
	}
}

void missileManager::launchMissile(){

	//choose missile target
	scene->MissilePosition();

	//launch missile and destroy piece if able
	destroyPiece();
}

void missileManager::destroyPiece() {

	switch (launchableMissile) {

		//checks to see what the RPG can destroy
	case RPG:
		console::error("Tried to launch RPG");
		scene->LaunchedMissile();
		
		//RPG Can only take pawns
		break;

		//checks to see what the ballistic missile can destroy
	case BALLISTIC:
		console::error("Tried to launch Ballistic Missile");
		scene->LaunchedMissile();
		//ballistic missiles can take pawns rooks and bishops

		break;

	//checks to see what the ICBM can destroy
	case ICBM:
		console::error("Tried to launch ICBM");
		scene->LaunchedMissile();
		//can take anything but the King
		break;

	//Whoever, launches this wins
	case VOYAGER:
		console::error("Tried to launch Voyager");
		scene->LaunchedMissile();
		//win the game
		break;

	//default case
	default:
		break;

	}
}
