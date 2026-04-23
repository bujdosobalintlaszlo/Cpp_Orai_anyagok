#pragma once
#include "game_state.h"

#include <filesystem>
#include <memory>

class World {
    struct Impl;
    std::unique_ptr<Impl> pimpl_;

   public:
    explicit World(const std::filesystem::path& pluginDir = "plugins");
    ~World();

    World(World&&) noexcept;
    World& operator=(World&&) noexcept;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void move(char direction, GameState& state);
    void reloadPlugins();
};
