#include "display.h"
#include "game_state.h"
#include "terminal_input.h"
#include "world.h"
#include "stat.h"

#include <iostream>

int main(int argc, char* argv[]) {
    World world;

    std::cout << "What's your name?\nCharacter Name: ";
    std::string name;
    std::cin >> name;

    GameState state(name, "Seyda Neen");
    state.saveToFile("save_1");

    displayCharacter(state.player.getName(), "Adventurer", 1, state.player.getHealth(),
                     state.player.getMaxHealth());

    bool is_running = true;
    int turn_number = 0;
    char last_input = '\0';

    Stat<std::string> titles({0}, "ddd");
    while (is_running) {
        if (last_input == '\0') {
            std::cout << "What should we do?\n";
            std::cout
                << "(i)nventory | (w)/(a)/(s)/(d) move | (v)iew | (r)eload plugins | (q)uit\n";
        } else {
            std::cout << "What should we do?\n";
        }

        last_input = TerminalInput::getCharTimeout(5000);


        switch (last_input) {
            case 'i':
                std::cout << "You rummage through your belongings.\n";
                state.player.inventory.display();
                break;
            case 'w':
            case 's':
            case 'a':
            case 'd':
                std::cout << "Titles: " << titles.getValue() << '\n';
                titles.modify("d");
                std::cout << "Titles: " << titles.getValue() << '\n';
                state.player.restoreMana(1);
                world.move(last_input, state);
                if (!state.player.isAlive()) {
                    std::cout << "Game Over. Rest in peace, " << state.player.getName() << ".\n";
                    is_running = false;
                }
                break;
            case 'v':
                std::cout << "You pause and survey your surroundings.\n";
                std::cout << "Location: " << state.location << "\n";
                state.player.displayStatus();
                break;
            case 'r':
                std::cout << "Reloading plugins...\n";
                world.reloadPlugins();
                break;
            case 'q':
                std::cout << "Farewell, " << state.player.getName()
                          << ". May your travels be safe.\n";
                is_running = false;
                break;
            case '\0':
                std::cout << "You stand still, listening to the wind.\n";
                break;
            default:
                std::cout << "That action is unknown to you.\n";
                break;
        }

        if (is_running) {
            std::cout << "\n~~~ Turn " << turn_number << " ends ~~~\n";
            ++turn_number;
        }
    }
}
