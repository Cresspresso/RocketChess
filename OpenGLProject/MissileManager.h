#pragma once


class missileManager {

public:

	int launchableMissile;
	bool missilePos;


	//to purchase missiles if the player has enough currency
	void purchaseMissile(int missileChoice);
	
	//launch a missile once the player has bought one
	void launchMissile();
	
	//checks to see if the target is fine
	bool checkMissileTarget();
	
	//checks to see if the position the 
	//missile was fired in destroys a piece
	void destroyPiece();


private:

	//missile types
	enum Missile {
		RPG,
		BALLISTIC,
		ICBM,
		VOYAGER

	};

};