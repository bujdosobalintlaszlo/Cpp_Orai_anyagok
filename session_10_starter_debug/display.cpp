#include "display.h"

void displayBar(int current, int max, int barWidth) {
    if (max == 0)
        max = 1;  // Avoid division by zero
    int filled = (current * barWidth) / max;
    if (filled > barWidth)
        filled = barWidth;  // Clamp to max

    std::cout << "[";
    for (int i = 0; i < barWidth; i++) {
        std::cout << (i < filled ? "█" : "░");
    }
    std::cout << "]";
}

void displayCharacter(const std::string& name, const std::string& charClass, int level, int health,
                      int maxHealth) {
    std::cout << "Name:   " << std::setw(15) << std::left << name << "\n";
    std::cout << "Class:  " << std::setw(15) << std::left << charClass << "\n";
    std::cout << "Level:  " << level << "\n";
    std::cout << "Health: ";
    displayBar(health, maxHealth);
    std::cout << " " << health << "/" << maxHealth << "\n";
}

void displayInBases(int value) {
    std::cout << "Dec: " << std::dec << value << ", ";
    std::cout << "Hex: " << std::hex << value << ", ";
    std::cout << "Oct: " << std::oct << value;
    std::cout << std::dec;  // Reset to decimal
}
