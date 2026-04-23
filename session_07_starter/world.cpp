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

// --- Everything below is hidden from anyone who includes world.h ---

struct World::Impl {
    std::mt19937 rng;
    std::filesystem::path pluginDir;

    // Each loaded plugin: library handle + factory function
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

        // Move-only
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

        for (const std::filesystem::directory_entry& entry :
             std::filesystem::directory_iterator(pluginDir)) {
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
#else
            lp.create = reinterpret_cast<CreateCreatureFunc>(dlsym(lp.handle, "createCreature"));
#endif
            if (!lp.create)
                continue;

#ifdef _WIN32
            lp.destroy = (DestroyCreatureFunc)GetProcAddress((HMODULE)lp.handle, "destroyCreature");
#else
            lp.destroy = reinterpret_cast<DestroyCreatureFunc>(dlsym(lp.handle, "destroyCreature"));
#endif
            if (!lp.destroy)
                continue;

            // Verify it works
            if (Creature* c = lp.create()) {
                std::cout << "Loaded creature plugin: " << c->getName() << "\n";
                delete c;
                plugins.push_back(std::move(lp));
            }
        }
    }

    using CreaturePtr = std::unique_ptr<Creature, void(*)(Creature*)>;

    static void defaultDelete(Creature* c) { delete c; }

    CreaturePtr spawnCreature() {
        int builtinCount = 3;
        int pluginCount = static_cast<int>(plugins.size());
        int total = builtinCount + pluginCount;

        int roll = std::uniform_int_distribution<int>(0, total - 1)(rng);

        if (roll < builtinCount) {
            switch (roll) {
                case 0:  return {new Rat(), defaultDelete};
                case 1:  return {new Mudcrab(), defaultDelete};
                default: return {new CliffRacer(), defaultDelete};
            }
        }

        return {plugins[roll - builtinCount].create(),
                plugins[roll - builtinCount].destroy};
    }

    bool shouldEncounter() { return std::uniform_int_distribution<int>(1, 5)(rng) <= 2; }
};

// --- Special members in .cpp where Impl is complete ---

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

    std::unique_ptr<Creature, void(*)(Creature*)> creature = pimpl_->spawnCreature();
    std::cout << "\n*** You encounter a " << creature->getName() << "! ***\n";
   // creature->encounter(state);
	 state.player.attack(*creature);
	 if(creature->getHealth() >0){
		  creature->attack(state.player);
	 }else{

		  if (Lootable* lootable = dynamic_cast<Lootable*>(creature.get())) {
				Item loot = lootable->getLoot();
				std::cout << "You loot: " << loot.name << " (worth " << loot.value << " gold)\n";
				state.player.inventory.addItem(loot.name, loot.value);
		  }

		  if (state.player.health <= 0) {
				state.player.health = 0;
				std::cout << "\nYou have fallen...\n";
		  }
	 }
	 

}
