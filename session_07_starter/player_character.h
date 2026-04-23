#pragma once


#include "entity.h"
#include "inventory.h"
class PlayerCharacter : public Entity {
	 Inventory inventory;
	 friend class GameState;
	 friend class World;
public: 
		  PlayerCharacter(const std::string& characterName,const std::string& characterClass ,int level,int gold,int health,int maxHealth,const std::string& location) 
				: Entity(characterName,characterClass ,level,gold,health,maxHealth,location)
				,inventory() {} 
	 void attack(Entity& target){
		  target-=level*5;
	 }
		  
};
