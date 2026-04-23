#pragma once
#include "creature.h"

// Creature plugins export these two C functions.
extern "C" {
typedef Creature* (*CreateCreatureFunc)();
typedef void (*DestroyCreatureFunc)(Creature*);
}

#ifdef _WIN32
#    define PLUGIN_EXPORT __declspec(dllexport)
#else
#    define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif
