#pragma once
#include <iostream>
#include <string>

#include "stat.h"


class Entity {
   protected:
    std::string name_;
    Stat<int> health_;
    int attackPower_;

   public:
    Entity(const std::string& name, int maxHealth, int attackPower)
        : name_(name), health_(maxHealth, maxHealth), attackPower_(attackPower) {}

    virtual ~Entity() = default;

    const std::string& getName() const { return name_; }
    int getHealth() const { return health_.getValue(); }
    int getMaxHealth() const { return health_.getMaxValue(); }
    int getAttackPower() const { return attackPower_; }
    bool isAlive() const { return health_.getValue() > 0; }

    void takeDamage(int amount) {
        health_.modify(-amount);
    }

    void heal(int amount) {
        health_.modify(amount);
    }

    virtual void attack(Entity& target) = 0;

    virtual void displayStatus() const {
        std::cout << name_ << " [HP: " << health_.getValue() << "/" << health_.getMaxValue() << "]\n";
    }
};
