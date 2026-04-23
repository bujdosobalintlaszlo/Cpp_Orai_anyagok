#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include "creature.h"

// Creature plugins export these two C functions.
// The World loads them at runtime via dlopen / LoadLibrary.
extern "C" {
typedef Creature* (*CreateCreatureFunc)();
typedef void (*DestroyCreatureFunc)(Creature*);
}

// Export macro for plugin shared libraries
#ifdef _WIN32
#    define PLUGIN_EXPORT __declspec(dllexport)
#else
#    define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

#endif
