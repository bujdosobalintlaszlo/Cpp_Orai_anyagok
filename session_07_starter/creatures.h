#pragma once
#include "creature.h"

#include <iostream>

// --- Rat: harmless, no loot ---
class Rat : public Creature {
   public:
    Rat() : Creature("Rat", 5) {}

    void encounter(GameState& state) override {
        std::cout << "A rat scurries across your path. It squeaks and runs away.\n";
    }
};

// --- Mudcrab: weak enemy, drops loot ---
class Mudcrab : public Creature, public Lootable {
   public:
    Mudcrab() : Creature("Mudcrab", 10) {}

    void encounter(GameState& state) override {
        std::cout << "A mudcrab snaps at your ankles! You kick it aside.\n";
        state.player.health -= 3;
        std::cout << "You take 3 damage. (HP: " << state.player.health << "/" << state.player.maxHealth << ")\n";
    }

    Item getLoot() const override { return Item("Mudcrab Chitin", 5); }
};

// --- CliffRacer: annoying, deals real damage ---
class CliffRacer : public Creature {
   public:
    CliffRacer() : Creature("Cliff Racer", 20) {}

    void encounter(GameState& state) override {
        std::cout << "A cliff racer swoops down from above with a horrible screech!\n";
        int damage = 8 + (state.player.level * 2);
        state.player.health -= damage;
        std::cout << "It claws you for " << damage << " damage! (HP: " << state.player.health << "/"
                  << state.player.maxHealth << ")\n";
    }

	 void attack(Enitity& target) const override {
		  target.take_damage(1);
	 }
};
