#include "plugin_interface.h"

#include <iostream>

class Scrib : public Creature, public Lootable {
   public:
    Scrib() : Creature("Scrib", 8, 3) {}

    void attack(Entity& target) override {
        std::cout << "The scrib stings you for " << attackPower_ << " damage!\n";
        target.takeDamage(attackPower_);
    }

    Item getLoot() const override { return Item("Scrib Jelly", 8); }
};

extern "C" {
PLUGIN_EXPORT Creature* createCreature() {
    return new Scrib();
}
PLUGIN_EXPORT void destroyCreature(Creature* c) {
    delete c;
}
}
