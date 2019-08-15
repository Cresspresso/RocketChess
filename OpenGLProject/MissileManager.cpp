#include "MissileManager.h"
#include "ChessPiece.hpp"
#include "Money.h"

//reference to Money.h
Money money;

void missileManager::purchaseMissile(int missileChoice){

	if (missileChoice == 1) {
		buyRPG();
	}
	if (missileChoice == 2) {
		buyBallistic();
	}
	if (missileChoice == 3) {
		buyICBM();
	}
	if (missileChoice == 4) {
		buyVoyager();
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
	choose a space on the board
	if(choice is out of range){
		return false;
	}
	else{
		missileTarget = chosen cell;
		return true;
	}
	*/


}

void missileManager::destroyPiece(){
	/*
		missileTarget
			if(piece == PAWN && launchableMissile == RPG){
				destroy piece 
				call gainMoney from Money.cpp
			}
	
	*/
}


void missileManager::buyRPG(){
	/*
	
	check to see whose turn it is
	check to see if the player has enough money
	
	buy missile
	
	*/

}

void missileManager::buyBallistic(){
	/*
	
	check to see whose turn it is
	check to see if the player has enough money

	buy missile
	
	*/
}

void missileManager::buyICBM(){

	/*
	
	check to see whose turn it is
	check to see if the player has enough money
	
	buy missile
	
	*/

}

void missileManager::buyVoyager(){

	/*
	check to see whose turn it is
	check to see if the player has enough money
	
	buy missile
	*/
	
}
