#include "plugin_interface.h"

#include <iostream>

class Netch : public Creature {
   public:
    Netch() : Creature("Betty Netch", 25, 8) {}

    void attack(Entity& target) override {
        std::cout << "The Betty Netch releases a cloud of poisonous spores for " << attackPower_
                  << " damage!\n";
        target.takeDamage(attackPower_);
    }
};

extern "C" {
PLUGIN_EXPORT Creature* createCreature() {
    return new Netch();
}
PLUGIN_EXPORT void destroyCreature(Creature* c) {
    delete c;
}
}
