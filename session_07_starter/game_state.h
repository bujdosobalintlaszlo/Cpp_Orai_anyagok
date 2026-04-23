#pragma once
#include "inventory.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include "player_character.h"
// StreamGuard: RAII class to save/restore stream formatting state
class StreamGuard {
    std::ios& stream_;
    std::ios::fmtflags flags_;
    std::streamsize precision_;
    char fill_;

   public:
    explicit StreamGuard(std::ios& stream)
        : stream_(stream), flags_(stream.flags()), precision_(stream.precision()),
          fill_(stream.fill()) {}

    ~StreamGuard() {
        stream_.flags(flags_);
        stream_.precision(precision_);
        stream_.fill(fill_);
    }

    // Prevent copying
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard& operator=(const StreamGuard&) = delete;
};

class GameState {
   public:
		 PlayerCharacter player;
        GameState(const std::string& name, const std::string& charClass, int level, int gold,
              const std::string& loc, int maxHp = 100)
        : player (name, charClass, level, gold, maxHp,maxHp,loc) {}

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << "\n";
            return false;
        }
		  /*
        // Write character data
        file << "#Character\n";
        file << characterName << "\n";
        file << characterClass << "\n";
        file << level << "\n";
        file << gold << "\n";
        file << location << "\n";

        // Write inventory
        file << inventory << "\n";
		  */
        return true;
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << filename << "\n";
            return false;
        }

        std::string buffer;

        if (!(file >> buffer)) {
            std::cerr << "Error: Invalid name\n";
            return false;
        }
        characterName = buffer;

        if (!(file >> buffer)) {
            std::cerr << "Error: Invalid class\n";
            return false;
        }
        characterClass = buffer;

        int level_read;
        if (!(file >> level_read) || level_read < 1 || level_read > 100) {
            std::cerr << "Error: Invalid level in save file\n";
            return false;
        }
        level = level_read;

        int gold_read;
        if (!(file >> gold_read) || gold_read < 0) {
            std::cerr << "Error: Invalid gold amount in save file\n";
            return false;
        }
        gold = gold_read;

        if (!(file >> buffer)) {
            std::cerr << "Error: Invalid location\n";
            return false;
        }
        location = buffer;

        Inventory inv_read;
        if (!(file >> inv_read)) {
            std::cerr << "Error: Invalid inventory\n";
            return false;
        }
        inventory = inv_read;

        return true;
    }

    void display() const {
        std::cout << "\nCharacter: " << characterName << "\n";
        std::cout << "Class: " << characterClass << "\n";
        std::cout << "Level: " << level << "\n";
        std::cout << "Gold: " << gold << "\n";
        std::cout << "Location: " << location << "\n";
        inventory.display();
        std::cout << "\n";
    }
};
