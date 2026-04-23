#pragma once
#include "entity.h"
#include "inventory.h"

#include "stat.h"

class Character : public Entity {
    Stat<double> mana_;

   public:
    Inventory inventory;

    Character(const std::string& name, int maxHealth, int attackPower, int maxMana)
        : Entity(name, maxHealth, attackPower), mana_(maxMana, maxMana) {}

    int getMana() const { return mana_.getValue(); }
    int getMaxMana() const { return mana_.getMaxValue(); }

    void useMana(int amount) {
      mana_.modify(-amount);
    }

    void restoreMana(int amount) {
      mana_.modify(amount);
    }

    void attack(Entity& target) override {
        std::cout << name_ << " strikes with a sword for " << attackPower_ << " damage!\n";
        target.takeDamage(attackPower_);
    }

    void displayStatus() const override {
        std::cout << name_ << " [HP: " << health_.getValue() << "/" << health_.getMaxValue() << " | MP: " << mana_.getValue() << "/"
                  << mana_.getMaxValue() << "]\n";
    }
};
