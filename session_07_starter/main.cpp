#include "display.h"
#include "game_state.h"
#include "terminal_input.h"
#include "world.h"

#include <chrono>
#include <iostream>
#include <thread>

void animate_blinker(std::chrono::milliseconds total_duration,
                     std::chrono::milliseconds blink_duration) {
    int num_dots_displayed = 0;
    while (total_duration.count() >= blink_duration.count()) {
        if (num_dots_displayed == 3) {
            std::cout << "\r   \r" << std::flush;
            num_dots_displayed = 0;
        } else {
            ++num_dots_displayed;
        }

        std::cout << '.' << std::flush;
        std::this_thread::sleep_for(blink_duration);
        total_duration -= blink_duration;
    }
    std::this_thread::sleep_for(total_duration);
    std::cout << '\r';
}

void play_intro() {
    using namespace std::chrono_literals;
    std::cout << "Wake up." << std::endl;
    animate_blinker(2760ms, 500ms);
    std::cout << "We're here." << std::endl;
    animate_blinker(3280ms, 500ms);
    std::cout << "Why are you shaking?" << std::endl;

    std::cout << std::endl;

    animate_blinker(760ms, 500ms);
    std::cout << "Are you okay?" << std::endl;
    animate_blinker(3760ms, 500ms);
    std::cout << "Wake up!" << std::endl;
    animate_blinker(1000ms, 500ms);
    std::cout << "Stand up." << std::endl;

    std::cout << std::endl;

    animate_blinker(1480ms, 500ms);
    std::cout << "There you go." << std::endl;
    animate_blinker(1519ms, 500ms);
    std::cout << "You were dreaming..." << std::endl;
    animate_blinker(2201ms, 500ms);
}

int main(int argc, char* argv[]) {
    World world;

    std::cout << "What's your name?\nCharacter Name: ";

    std::string name;
    std::cin >> name;

    GameState state(name, "unknown", 1, 0, "HereAndNow");
    state.saveToFile("save_1");

   // displayCharacter(state.characterName, state.characterClass, state.level, 100, 100);

    bool is_running = true;
    uint32 turn_number = 0;
    char last_input = '\0';
    while (is_running) {
        if (last_input == '\0') {
            std::cout << "What should we do?\n";
            std::cout << "(i)nventory | (l)oot | (w)/(a)/(s)/(d) move | (v)iew surroundings | "
                         "(r)eload plugins | (q)uit\n";
        } else {
            std::cout << "What should we do?\n";
        }

        last_input = TerminalInput::getCharTimeout(5000);

        switch (last_input) {
            case 'i':
                std::cout << "You rummage through your belongings.\n";
                std::cout << "Inventory: " << state.inventory << "\n";
                break;
            case 'l':
                std::cout << "You search the area and find something useful!\n";
                break;
            case 'w':
            case 's':
            case 'a':
            case 'd':
                world.move(last_input, state);
                if (state.player.health <= 0) {
                  //  std::cout << "Game Over. Rest in peace, " << state.characterName << ".\n";
                    is_running = false;
                }
                break;
            case 'v':
                std::cout << "You pause and survey your surroundings carefully.\n";
                std::cout << "You are in " << state.player.location << "\n";
              //  displayCharacter(state.characterName, state.characterClass, state.level,
                                 state.player.health, state.player.maxHealth);
                break;
            case 'r':
                std::cout << "Reloading plugins...\n";
                world.reloadPlugins();
                break;
            case 'q':
               // std::cout << "Farewell, " << state.characterName << ". May your travels be safe.\n";
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
