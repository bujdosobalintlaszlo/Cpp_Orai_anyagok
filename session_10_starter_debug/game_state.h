#pragma once
#include "character.h"

#include <fstream>
#include <iostream>
#include <string>

class GameState {
   public:
    Character player;
    std::string location;
    int gold;

    GameState(const std::string& name, const std::string& loc, int maxHp = 100,
              int attackPower = 12, int maxMana = 50)
        : player(name, maxHp, attackPower, maxMana), location(loc), gold(0) {}

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            return false;

        file << player.getName() << "\n";
        file << player.getHealth() << "\n";
        file << player.getMaxHealth() << "\n";
        file << gold << "\n";
        file << location << "\n";
        file << player.inventory << "\n";
        return true;
    }
};
