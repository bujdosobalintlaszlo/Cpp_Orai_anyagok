#pragma once
#include "game_state.h"
#include "entity.h"
#include <string>

// --- Abstract base class (Session 07: Inheritance, Session 08: Polymorphism) ---
// Pure interface: every creature must define how an encounter plays out.
class Creature : public Entity {
   friend class GameState;
   public:
    Creature(const std::string& name, int health) : Entity(name,"",1,0,health,health,"") {}

    virtual ~Creature() = default;

    const std::string& getName() const { return characterName; }
    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }

    // Pure virtual — each creature defines its own encounter behaviour
    virtual void encounter(GameState& state) = 0;

	 void take_damage(int amount) override {
		  health -= amount;
	 }

};

// --- Interface for lootable creatures (Session 09 preview: multiple inheritance) ---
class Lootable {
   public:
    virtual ~Lootable() = default;
    virtual Item getLoot() const = 0;
};
