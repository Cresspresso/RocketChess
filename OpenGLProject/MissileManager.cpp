
#include "MissileManager.h"
#include "ChessPiece.hpp"
#include "Money.h"

//reference to Money.h
Money money;

void missileManager::purchaseMissile(int missileChoice){

	//switch case for each missile type
	switch (missileChoice) {
	//check to see whose turn it is
	
	//rpg missile
	case 1:
		if (money.p1Money >= 3) {
			launchableMissile = RPG;
			money.p1Money -= 3;
			//move to choosing missile launch position
			launchMissile();
		}
		break;

	//default case
	default:
		break;
	
	}
}

void missileManager::launchMissile(){

	while (missilePos == false) {

		//choose missile target
		missilePos = checkMissileTarget();

		if (missilePos == true) {
			break;
		}	
	}
	//launch missile and destroy piece if able
	destroyPiece();
}


//choose position on grid to launch missile
bool missileManager::checkMissileTarget(){
	
	
	/*
	TODO Waiting on grid

	choose a space on the board
	if(choice is out of range){
		print("was not in range")
		return false;
	}
	else{
		missileTarget = chosen cell;
		return true;
	}
	*/

	return false;
}

void missileManager::destroyPiece() {

	switch (launchableMissile) {

		//checks to see what the RPG can destroy
	case RPG:

		/*
		RPG Can only take pawns
		check to see what piece is in the chosen cell
		if it can be taken, take it
		other return as a miss
		*/

		break;

		//checks to see what the ballistic missile can destroy
	case BALLISTIC:
		//ballistic missiles can take pawns rooks and bishops

		break;

	//checks to see what the ICBM can destroy
	case ICBM:
		//can take anything but the King
		break;

	//Whoever, launches this wins
	case VOYAGER:
		//win the game
		break;

	//default case
	default:
		break;

	}
}
