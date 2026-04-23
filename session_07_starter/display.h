#pragma once
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

// ============================================================================
// REQUIRED FUNCTIONS FOR TESTS
// ============================================================================
// Implement these functions to pass the automated tests.

/**
 * Display a progress bar
 * Requirements for tests:
 * - Print exactly: [filled_chars][empty_chars]
 * - Use '█' (U+2588) for filled, '░' (U+2591) for empty
 * - Calculate filled = (current * barWidth) / max
 */
void displayBar(int current, int max, int barWidth = 10);

/**
 * Display character information
 * Requirements: Must call displayBar() and use std::setw()
 */
void displayCharacter(const std::string& name, const std::string& charClass, int level, int health,
                      int maxHealth);

/**
 * Display a number in different bases
 * Requirements: Output in decimal, hexadecimal, and octal
 */
void displayInBases(int value);

/**
 * Display size and limits of a type
 * Requirements: Display type name, size, min, and max
 */
template <typename T>
void displayTypeInfo(const std::string& typeName) {
    // TODO: Implement type info display
    std::cout << "Type: " << typeName << "\n";
    std::cout << "  Size: " << sizeof(T) << " bytes\n";
    std::cout << "  Min:  " << std::numeric_limits<T>::min() << "\n";
    std::cout << "  Max:  " << std::numeric_limits<T>::max() << "\n";
}
