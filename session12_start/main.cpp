#include "game.h"

int main() {
    GameEngine game;
    game.initialize();
    game.run();
    game.shutdown();
    return 0;
}
