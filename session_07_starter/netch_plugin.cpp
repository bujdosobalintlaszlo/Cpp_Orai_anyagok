#include "plugin_interface.h"
#include <iostream>

class Netch : public Creature {
public:
    Netch() : Creature("Betty Netch", 30) {}

    void encounter(GameState& state) override {
        std::cout << "A Betty Netch drifts overhead... it looks angry!\n";
        std::cout << "It releases a cloud of poisonous spores!\n";
        state.health -= 25;
        std::cout << "You take 25 damage! (HP: " << state.health
                  << "/" << state.maxHealth << ")\n";
    }
};

extern "C" {
PLUGIN_EXPORT Creature* createCreature() { return new Netch(); }
PLUGIN_EXPORT void destroyCreature(Creature* c) { delete c; }
}
