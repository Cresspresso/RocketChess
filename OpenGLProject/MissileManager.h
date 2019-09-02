#pragma once

#include <array>
#include <map>
#include <optional>

struct Scene;

class missileManager {

public:

	int launchableMissile;
	int missilePos;
	Scene* scene{};

	//to purchase missiles if the player has enough currency
	void purchaseMissile(int missileChoice);
	
	//launch a missile once the player has bought one
	void launchMissile();
	
	//checks to see if the position the 
	//missile was fired in destroys a piece
	void destroyPiece(int missilePos);


private:

	//missile types
	enum Missile {
		RPG,
		BALLISTIC,
		ICBM,
		VOYAGER

	};

};

inline missileManager missile;
