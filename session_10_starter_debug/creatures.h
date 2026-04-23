#pragma once
#include "creature.h"

#include <iostream>

class Rat : public Creature {
   public:
    Rat() : Creature("Rat", 5, 1) {}

    void attack(Entity& target) override {
        std::cout << "The rat squeaks and nibbles at you for " << attackPower_ << " damage!\n";
        target.takeDamage(attackPower_);
    }
};

class Mudcrab : public Creature, public Lootable {
   public:
    Mudcrab() : Creature("Mudcrab", 12, 4) {}

    void attack(Entity& target) override {
        std::cout << "The mudcrab snaps its claws for " << attackPower_ << " damage!\n";
        target.takeDamage(attackPower_);
    }

    Item getLoot() const override { return Item("Mudcrab Chitin", 5); }
};

class CliffRacer : public Creature {
   public:
    CliffRacer() : Creature("Cliff Racer", 20, 10) {}

    void attack(Entity& target) override {
        std::cout << "The cliff racer swoops and claws for " << attackPower_ << " damage!\n";
        target.takeDamage(attackPower_);
    }
};
