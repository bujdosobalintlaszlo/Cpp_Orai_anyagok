#include "world.h"

#include "creature.h"
#include "creatures.h"
#include "plugin_interface.h"

#include <filesystem>
#include <iostream>
#include <random>
#include <vector>

#ifdef _WIN32
#    include <windows.h>
#else
#    include <dlfcn.h>
#endif

struct World::Impl {
    std::mt19937 rng;
    std::filesystem::path pluginDir;

    struct LoadedPlugin {
        void* handle = nullptr;
        CreateCreatureFunc create = nullptr;
        DestroyCreatureFunc destroy = nullptr;

        ~LoadedPlugin() {
            if (handle) {
#ifdef _WIN32
                FreeLibrary((HMODULE)handle);
#else
                dlclose(handle);
#endif
            }
        }

        LoadedPlugin() = default;
        LoadedPlugin(LoadedPlugin&& o) noexcept
            : handle(o.handle), create(o.create), destroy(o.destroy) {
            o.handle = nullptr;
            o.create = nullptr;
            o.destroy = nullptr;
        }
        LoadedPlugin& operator=(LoadedPlugin&&) = delete;
        LoadedPlugin(const LoadedPlugin&) = delete;
        LoadedPlugin& operator=(const LoadedPlugin&) = delete;
    };

    std::vector<LoadedPlugin> plugins;

    explicit Impl(const std::filesystem::path& dir) : rng(std::random_device{}()), pluginDir(dir) {
        loadPlugins();
    }

    void loadPlugins() {
        plugins.clear();
        if (!std::filesystem::exists(pluginDir))
            return;

        for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
            if (!entry.is_regular_file())
                continue;

            std::string ext = entry.path().extension().string();
#ifdef _WIN32
            if (ext != ".dll")
                continue;
#elif __APPLE__
            if (ext != ".dylib")
                continue;
#else
            if (ext != ".so")
                continue;
#endif

            LoadedPlugin lp;
#ifdef _WIN32
            lp.handle = LoadLibraryA(entry.path().string().c_str());
#else
            lp.handle = dlopen(entry.path().string().c_str(), RTLD_LAZY);
#endif
            if (!lp.handle)
                continue;

#ifdef _WIN32
            lp.create = (CreateCreatureFunc)GetProcAddress((HMODULE)lp.handle, "createCreature");
            lp.destroy = (DestroyCreatureFunc)GetProcAddress((HMODULE)lp.handle, "destroyCreature");
#else
            lp.create = reinterpret_cast<CreateCreatureFunc>(dlsym(lp.handle, "createCreature"));
            lp.destroy = reinterpret_cast<DestroyCreatureFunc>(dlsym(lp.handle, "destroyCreature"));
#endif
            if (!lp.create || !lp.destroy)
                continue;

            if (Creature* c = lp.create()) {
                std::cout << "Loaded creature plugin: " << c->getName() << "\n";
                lp.destroy(c);
                plugins.push_back(std::move(lp));
            }
        }
    }

    using CreaturePtr = std::unique_ptr<Creature, void (*)(Creature*)>;

    static void defaultDelete(Creature* c) { delete c; }

    CreaturePtr spawnCreature() {
        int builtinCount = 3;
        int pluginCount = static_cast<int>(plugins.size());
        int total = builtinCount + pluginCount;

        int roll = std::uniform_int_distribution<int>(0, total - 1)(rng);

        if (roll < builtinCount) {
            switch (roll) {
                case 0:
                    return {new Rat(), defaultDelete};
                case 1:
                    return {new Mudcrab(), defaultDelete};
                default:
                    return {new CliffRacer(), defaultDelete};
            }
        }

        return {plugins[roll - builtinCount].create(), plugins[roll - builtinCount].destroy};
    }

    bool shouldEncounter() { return std::uniform_int_distribution<int>(1, 5)(rng) <= 2; }

    // --- Session 08: Real combat loop ---
    void combat(Character& player, Creature& creature) {
        std::cout << "\nCOMBAT: " << player.getName() << " vs " << creature.getName() << "!\n";
        player.displayStatus();
        creature.displayStatus();
        std::cout << "\n";

        while (player.isAlive() && creature.isAlive()) {
            // Player attacks first
            player.attack(creature);
            creature.displayStatus();

            if (!creature.isAlive()) {
                std::cout << "\n" << creature.getName() << " defeated!\n";
                break;
            }

            // Creature attacks back — polymorphic dispatch
            creature.attack(player);
            player.displayStatus();
            std::cout << "\n";
        }
    }
};

World::World(const std::filesystem::path& pluginDir) : pimpl_(std::make_unique<Impl>(pluginDir)) {}
World::~World() = default;
World::World(World&&) noexcept = default;
World& World::operator=(World&&) noexcept = default;

void World::reloadPlugins() {
    pimpl_->loadPlugins();
}

void World::move(char direction, GameState& state) {
    const char* dirName = nullptr;
    switch (direction) {
        case 'w':
            dirName = "north";
            break;
        case 's':
            dirName = "south";
            break;
        case 'a':
            dirName = "west";
            break;
        case 'd':
            dirName = "east";
            break;
        default:
            return;
    }

    std::cout << "You travel " << dirName << ".\n";

    if (!pimpl_->shouldEncounter()) {
        std::cout << "The road is quiet.\n";
        return;
    }

    auto creature = pimpl_->spawnCreature();
    std::cout << "\n*** You encounter a " << creature->getName() << "! ***\n";

    // Real combat
    pimpl_->combat(state.player, *creature);

    // Loot check after victory
    if (!creature->isAlive()) {
        if (auto* lootable = dynamic_cast<Lootable*>(creature.get())) {
            Item loot = lootable->getLoot();
            std::cout << "You loot: " << loot.name << " (worth " << loot.value << " gold)\n";
            state.player.inventory.addItem(loot.name, loot.value);
        }
    }

    if (!state.player.isAlive()) {
        std::cout << "\nYou have fallen...\n";
    }
}
