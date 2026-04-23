#pragma once
#include <string>

class Entity{
	public:
	 std::string characterName;
	 std::string characterClass;
	 int level;
	 int gold;
	 int health;
	 int maxHealth;
	 std::string location;

	 virtual void take_damage(int amount) = 0;
	 //csak azert mert most nem modosit semmit a hivo structjan belul
	 virtual void attack(Entity& target) const = 0;		  
	 Entity(const std::string& characterName,const std::string& characterClass ,int level,int gold,int health,int maxHealth,const std::string& location) : 
		  characterName(characterName),
		  characterClass(characterClass),
		  level(level),
		  gold(gold),
		  health(health),
		  maxHealth(maxHealth),
		  location(location)
		  {}

	virtual ~Entity() =default;
};
