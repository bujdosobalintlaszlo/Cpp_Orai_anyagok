#pragma once
#include "entity.h"
#include "inventory.h"

#include <string>

class Creature : public Entity {
   public:
    Creature(const std::string& name, int maxHealth, int attackPower)
        : Entity(name, maxHealth, attackPower) {}
};

class Lootable {
   public:
    virtual ~Lootable() = default;
    virtual Item getLoot() const = 0;
};
