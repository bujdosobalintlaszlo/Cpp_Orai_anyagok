#include "plugin_interface.h"
#include "entity.h"
#include <iostream>

class Scrib : public Creature, public Lootable {
   public:
    Scrib() : Creature("Scrib", 8) {}

    void encounter(GameState& state) override {
        std::cout << "A scrib charges at you with its tiny legs!\n";
        std::cout << "You squash it easily, but it stings you first.\n";
        state.player.health -= 2;
        std::cout << "You take 2 damage. (HP: " << state.health << "/" << state.maxHealth << ")\n";

    }

    Item getLoot() const override { return Item("Scrib Jelly", 8); }
	 void attack(Enitity& targer) const override {
		  target.health-=1;
	 }
		  
};

extern "C" {
PLUGIN_EXPORT Creature* createCreature() {
    return new Scrib();
}
PLUGIN_EXPORT void destroyCreature(Creature* c) {
    delete c;
}
}
