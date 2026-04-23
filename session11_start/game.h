#pragma once

// ============================================================================
// SESSION 11 START — Snapshot of game_world with sessions 1–10 applied
//
// This is the game as it stands. Students already know vector, map, unique_ptr,
// classes, inheritance, polymorphism, templates. The code below uses all of those.
//
// NEW FOR SESSION 11:
//   - stl_game.h: Quest system + algorithm free functions (students implement)
//   - ALGORITHM OPPORTUNITY comments below mark places where STL algorithms
//     could replace manual loops in the existing game code
// ============================================================================

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

// ============================================================================
// Session 2: Item + Inventory (raw pointer management)
// ============================================================================

class Item {
   private:
    std::string name_;
    int value_;

   public:
    Item(const std::string& name, int value) : name_(name), value_(value) {}
    const std::string& getName() const { return name_; }
    int getValue() const { return value_; }
};

class Inventory {
   private:
    Item** items_;
    int capacity_;
    int count_;

   public:
    Inventory(int capacity) : capacity_(capacity), count_(0) {
        items_ = new Item*[capacity_];
        for (int i = 0; i < capacity_; i++) items_[i] = nullptr;
    }

    ~Inventory() {
        for (int i = 0; i < count_; i++) delete items_[i];
        delete[] items_;
    }

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    bool addItem(const std::string& name, int value) {
        if (count_ >= capacity_) return false;
        items_[count_] = new Item(name, value);
        count_++;
        return true;
    }

    // ALGORITHM OPPORTUNITY: this manual search + shift could be
    // std::find_if on a vector, then erase
    bool removeItem(const std::string& name) {
        for (int i = 0; i < count_; i++) {
            if (items_[i] && items_[i]->getName() == name) {
                delete items_[i];
                for (int j = i; j < count_ - 1; j++) items_[j] = items_[j + 1];
                items_[count_ - 1] = nullptr;
                count_--;
                return true;
            }
        }
        return false;
    }
	
	 /*
	  * C++11 es alatt, ezt a lamda legeneralja maganak a hatterben
	 struct EqualByName{
		  const std::string& name;
		  EqualByName(const std::string& name) : name(name) {}
		  
		  bool operator()(Item* item){
				return item && item->getName() == name;
		  }
	 };
	 */
    // ALGORITHM OPPORTUNITY: std::find_if
    Item* findItem(const std::string& name) {
		  /*
        for (int i = 0; i < count_; i++) {
            if (items_[i] && items_[i]->getName() == name) return items_[i];
        }
        return nullptr;
		  */
		  Item **pos =std::find_if(items_,items_+count_,[&name](Item * item){return item && item->getName() == name;});
		  if(pos == items_+count_) return nullptr;
		  return *pos;
    }

    void display() const {
        std::cout << "\n🎒 Inventory (" << count_ << "/" << capacity_ << " items):\n";
        if (count_ == 0) {
            std::cout << "   (empty)\n";
            return;
        }
        for (int i = 0; i < count_; i++) {
            if (items_[i]) {
                std::cout << "   " << (i + 1) << ". " << std::left << std::setw(20)
                          << items_[i]->getName() << " - " << items_[i]->getValue() << " gold\n";
            }
        }
    }

    int getCount() const { return count_; }
    int getCapacity() const { return capacity_; }
};

// ============================================================================
// Session 4: Weapon (smart pointers, move-only)
// ============================================================================

class Weapon {
   private:
    std::string name_;
    int damage_;

   public:
    Weapon(const std::string& name, int damage) : name_(name), damage_(damage) {}
    ~Weapon() = default;

    Weapon(Weapon&&) noexcept = default;
    Weapon& operator=(Weapon&&) noexcept = default;
    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    const std::string& getName() const { return name_; }
    int getDamage() const { return damage_; }
};

// ============================================================================
// Session 8: Entity hierarchy (polymorphism)
// ============================================================================

class Entity {
   protected:
    std::string name_;
    int health_;
    int maxHealth_;
    int attackPower_;

   public:
    Entity(const std::string& name, int maxHealth, int attackPower)
        : name_(name), health_(maxHealth), maxHealth_(maxHealth), attackPower_(attackPower) {}

    virtual ~Entity() = default;

    const std::string& getName() const { return name_; }
    int getHealth() const { return health_; }
    bool isAlive() const { return health_ > 0; }
    void takeDamage(int amount) { health_ = std::max(0, health_ - amount); }

    virtual void attack(Entity* target) = 0;
    virtual void defend() = 0;
    virtual std::string getType() const = 0;
};

class Warrior : public Entity {
   private:
    int armor_;

   public:
    Warrior(const std::string& name, int health, int attackPower, int armor)
        : Entity(name, health, attackPower), armor_(armor) {}

    void attack(Entity* target) override { target->takeDamage(attackPower_); }
    void defend() override {}
    std::string getType() const override { return "Warrior"; }
    int getArmor() const { return armor_; }
};

class Mage : public Entity {
   private:
    int mana_;

   public:
    Mage(const std::string& name, int health, int attackPower, int mana)
        : Entity(name, health, attackPower), mana_(mana) {}

    void attack(Entity* target) override {
        if (mana_ >= 10) {
            target->takeDamage(attackPower_);
            mana_ -= 10;
        }
    }
    void defend() override {}
    std::string getType() const override { return "Mage"; }
    int getMana() const { return mana_; }
};

// ============================================================================
// Location (same as game_world — already uses vector + map)
// ============================================================================

struct Location {
    std::string name;
    std::string description;
    std::unique_ptr<Entity> enemy;
    std::vector<std::string> treasureNames;
    std::vector<int> treasureValues;
    bool visited;
    std::map<char, int> exits;

    Location(const std::string& n, const std::string& desc)
        : name(n), description(desc), visited(false) {}
};

// ============================================================================
// GameEngine — snapshot of game_world with sessions 1–10
//
// ALGORITHM OPPORTUNITY comments mark where STL algorithms could help.
// The quest system (#include "stl_game.h") is the new session 11 work.
// ============================================================================

#include "stl_game.h"

class GameEngine {
   private:
    bool running_;

    std::string playerName_;
    int playerHealth_;
    int playerMaxHealth_;
    int playerAttack_;
    int playerGold_;
    int playerLevel_;
    std::string currentLocationName_;

    std::unique_ptr<Inventory> inventory_;
    std::unique_ptr<Weapon> equippedWeapon_;
    std::unique_ptr<QuestManager> questManager_;

    std::vector<std::unique_ptr<Location>> dungeon_;
    int currentLocation_;
    bool bossDefeated_;

    std::mt19937 rng_;

   public:
    GameEngine()
        : running_(false),
          playerName_("Hero"),
          playerHealth_(100),
          playerMaxHealth_(100),
          playerAttack_(15),
          playerGold_(0),
          playerLevel_(1),
          currentLocationName_("Dungeon Entrance"),
          currentLocation_(0),
          bossDefeated_(false),
          rng_(std::random_device{}()) {
        inventory_ = std::make_unique<Inventory>(20);
        questManager_ = std::make_unique<QuestManager>();
        createDungeon();
        initializeQuests();
    }

    // Same dungeon as game_world
    void createDungeon() {
        auto loc0 = std::make_unique<Location>(
            "Dungeon Entrance",
            "You stand at the entrance of a dark dungeon. Torches flicker on the walls.");
        loc0->exits['n'] = 1;
        dungeon_.push_back(std::move(loc0));

        auto loc1 = std::make_unique<Location>(
            "Grand Hall",
            "A vast hall with crumbling pillars. You hear echoes in the distance.");
        loc1->exits['s'] = 0;
        loc1->exits['e'] = 2;
        loc1->exits['w'] = 3;
        loc1->exits['n'] = 4;
        loc1->enemy = std::make_unique<Warrior>("Goblin Scout", 30, 8, 5);
        loc1->treasureNames.push_back("Rusty Dagger");
        loc1->treasureValues.push_back(10);
        dungeon_.push_back(std::move(loc1));

        auto loc2 = std::make_unique<Location>(
            "Old Armory", "Broken weapons and armor litter the floor.");
        loc2->exits['w'] = 1;
        loc2->treasureNames.push_back("Iron Sword");
        loc2->treasureValues.push_back(50);
        loc2->treasureNames.push_back("Leather Armor");
        loc2->treasureValues.push_back(40);
        dungeon_.push_back(std::move(loc2));

        auto loc3 = std::make_unique<Location>(
            "Storage Room", "Dusty crates and barrels fill this room.");
        loc3->exits['e'] = 1;
        loc3->treasureNames.push_back("Health Potion");
        loc3->treasureValues.push_back(25);
        loc3->treasureNames.push_back("Gold Coins");
        loc3->treasureValues.push_back(100);
        dungeon_.push_back(std::move(loc3));

        auto loc4 = std::make_unique<Location>(
            "Guard Room",
            "This room once housed the dungeon guards. Bones scatter the floor.");
        loc4->exits['s'] = 1;
        loc4->exits['n'] = 5;
        loc4->enemy = std::make_unique<Warrior>("Skeleton Warrior", 50, 12, 8);
        loc4->treasureNames.push_back("Steel Sword");
        loc4->treasureValues.push_back(100);
        dungeon_.push_back(std::move(loc4));

        auto loc5 = std::make_unique<Location>(
            "Treasure Chamber", "Gold and jewels glitter in the torchlight!");
        loc5->exits['s'] = 4;
        loc5->exits['n'] = 6;
        loc5->treasureNames.push_back("Magic Amulet");
        loc5->treasureValues.push_back(200);
        loc5->treasureNames.push_back("Gold Pile");
        loc5->treasureValues.push_back(500);
        dungeon_.push_back(std::move(loc5));

        auto loc6 = std::make_unique<Location>(
            "Dragon's Lair",
            "A massive chamber. The air is thick with smoke and the smell of sulfur.");
        loc6->exits['s'] = 5;
        loc6->enemy = std::make_unique<Mage>("Ancient Dragon", 150, 25, 100);
        loc6->treasureNames.push_back("Dragon Hoard");
        loc6->treasureValues.push_back(5000);
        dungeon_.push_back(std::move(loc6));
    }

    // Same quest IDs as game_world
    void initializeQuests() {
        questManager_->addQuest({"goblin", "Defeat the Goblin Scout", false});
        questManager_->addQuest({"skeleton", "Defeat the Skeleton Warrior", false});
        questManager_->addQuest({"treasure", "Find the Magic Amulet", false});
        questManager_->addQuest({"dragon", "Slay the Ancient Dragon", false});
    }

    void initialize() {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     C++ QUEST: DUNGEON CRAWLER         ║\n";
        std::cout << "╚════════════════════════════════════════╝\n\n";

        std::cout << "You are " << playerName_ << ", a brave adventurer.\n";
        std::cout << "A dark dungeon awaits. Treasure and danger lie within!\n\n";

        std::cout << "Commands:\n";
        std::cout << "  n/s/e/w - Move north/south/east/west\n";
        std::cout << "  look    - Examine current location\n";
        std::cout << "  fight   - Fight enemy in current location\n";
        std::cout << "  flee    - Run from combat\n";
        std::cout << "  loot    - Take treasure from current location\n";
        std::cout << "  stats   - View your character\n";
        std::cout << "  inv     - View inventory\n";
        std::cout << "  quests  - View quests\n";
        std::cout << "  quit    - Exit game\n\n";

        running_ = true;
        describeLocation();
    }

    void run() {
        if (!running_) return;
        while (running_) {
            std::cout << "\n> ";
            std::string command;
            std::cin >> command;

            if (command == "n" || command == "north") move('n');
            else if (command == "s" || command == "south") move('s');
            else if (command == "e" || command == "east") move('e');
            else if (command == "w" || command == "west") move('w');
            else if (command == "look") describeLocation();
            else if (command == "fight") fight();
            else if (command == "flee") flee();
            else if (command == "loot") loot();
            else if (command == "stats") showStats();
            else if (command == "inv") inventory_->display();
            else if (command == "quests") showQuests();
            else if (command == "quit") running_ = false;
            else std::cout << "Unknown command. Type 'look' for help.\n";

            if (bossDefeated_) {
                std::cout << "\n╔════════════════════════════════════════╗\n";
                std::cout << "║          VICTORY!                      ║\n";
                std::cout << "╚════════════════════════════════════════╝\n";
                std::cout << "You have defeated the Ancient Dragon!\n";
                showStats();
                running_ = false;
            }
            if (playerHealth_ <= 0) {
                std::cout << "\n╔════════════════════════════════════════╗\n";
                std::cout << "║          GAME OVER                     ║\n";
                std::cout << "╚════════════════════════════════════════╝\n";
                std::cout << "You have fallen in the dungeon...\n";
                running_ = false;
            }
        }
    }

    void shutdown() {
        std::cout << "\nThanks for playing C++ Quest!\n";
        std::cout << "Keep learning and building! 🚀\n\n";
    }

   private:
    void checkQuestCompletion(const std::string& questId) {
        if (!questManager_->isCompleted(questId)) {
            questManager_->completeQuest(questId);
            std::cout << "\n🎯 Quest Completed!\n";
        }
    }

    void describeLocation() {
        Location& loc = *dungeon_[currentLocation_];
        currentLocationName_ = loc.name;

        std::cout << "\n═══════════════════════════════════\n";
        std::cout << loc.name << "\n";
        std::cout << "═══════════════════════════════════\n";
        std::cout << loc.description << "\n";

        if (loc.enemy && loc.enemy->isAlive()) {
            std::cout << "\n⚠️  " << loc.enemy->getName() << " blocks your path!\n";
            std::cout << "   Type: " << loc.enemy->getType() << "\n";
            std::cout << "   HP: " << loc.enemy->getHealth() << "\n";
        }

        // ALGORITHM OPPORTUNITY: could use std::for_each or range-based
        // with std::transform to format treasure display
        if (!loc.treasureNames.empty()) {
            std::cout << "\n✨ You see treasure here:\n";
				for (size_t i = 0; i < loc.treasureNames.size(); ++i) {
                std::cout << "   - " << loc.treasureNames[i]
                          << " (" << loc.treasureValues[i] << " gold)\n";
            }
        }

        std::cout << "\nExits: ";
        for (const auto& [dir, _] : loc.exits) {
            switch (dir) {
                case 'n': std::cout << "north "; break;
                case 's': std::cout << "south "; break;
                case 'e': std::cout << "east "; break;
                case 'w': std::cout << "west "; break;
            }
        }
        std::cout << "\n";
        loc.visited = true;
    }

    void move(char direction) {
        Location& loc = *dungeon_[currentLocation_];
        if (loc.enemy && loc.enemy->isAlive()) {
            std::cout << "You cannot leave while " << loc.enemy->getName()
                      << " blocks your path!\n";
            return;
        }
        auto it = loc.exits.find(direction);
        if (it == loc.exits.end()) {
            std::cout << "You cannot go that way.\n";
            return;
        }
        currentLocation_ = it->second;
        describeLocation();
    }

    void fight() {
        Location& loc = *dungeon_[currentLocation_];
        if (!loc.enemy || !loc.enemy->isAlive()) {
            std::cout << "There is nothing to fight here.\n";
            return;
        }

        Entity* enemy = loc.enemy.get();
        std::cout << "\n⚔️  COMBAT: You vs " << enemy->getName()
                  << " (" << enemy->getType() << ")\n\n";

        while (enemy->isAlive() && playerHealth_ > 0) {
            int totalAttack = playerAttack_;
            if (equippedWeapon_) totalAttack += equippedWeapon_->getDamage();

            std::uniform_int_distribution<int> dist(0, 4);
            int damage = totalAttack + dist(rng_);
            enemy->takeDamage(damage);

            std::cout << "You attack for " << damage << " damage! ("
                      << enemy->getName() << " HP: " << enemy->getHealth() << ")\n";

            if (!enemy->isAlive()) {
                std::cout << "\n🎉 Victory! " << enemy->getName() << " defeated!\n";

                if (enemy->getName() == "Goblin Scout") checkQuestCompletion("goblin");
                else if (enemy->getName() == "Skeleton Warrior") checkQuestCompletion("skeleton");
                else if (enemy->getName() == "Ancient Dragon") checkQuestCompletion("dragon");

                if (enemy->getType() == "Mage" && enemy->getName() == "Ancient Dragon")
                    bossDefeated_ = true;
                break;
            }

            std::uniform_int_distribution<int> edist(0, 4);
            int enemyDamage = 10 + edist(rng_);
            playerHealth_ = std::max(0, playerHealth_ - enemyDamage);

            std::cout << enemy->getName() << " hits you for " << enemyDamage
                      << "! (HP: " << playerHealth_ << "/" << playerMaxHealth_ << ")\n\n";
        }
    }

    void flee() {
        Location& loc = *dungeon_[currentLocation_];
        if (!loc.enemy || !loc.enemy->isAlive()) {
            std::cout << "There is nothing to flee from.\n";
            return;
        }
        playerHealth_ = std::max(0, playerHealth_ - 5);
        std::cout << "You flee! " << loc.enemy->getName() << " strikes you as you run! (-5 HP)\n";
        currentLocation_ = 0;
        describeLocation();
    }

    void loot() {
        Location& loc = *dungeon_[currentLocation_];
        if (loc.treasureNames.empty()) {
            std::cout << "There is no treasure here.\n";
            return;
        }

        std::cout << "\n💰 You collect:\n";
        for (size_t i = 0; i < loc.treasureNames.size(); ++i) {
            const std::string& tname = loc.treasureNames[i];
            int value = loc.treasureValues[i];

            inventory_->addItem(tname, value);
            playerGold_ += value;

            if (tname == "Magic Amulet") checkQuestCompletion("treasure");

            if (tname.find("Sword") != std::string::npos ||
                tname.find("Dagger") != std::string::npos) {
                int weaponDamage = value / 10;
                equippedWeapon_ = std::make_unique<Weapon>(tname, weaponDamage);
                std::cout << "   ⚔️  Equipped " << tname << " (+" << weaponDamage << " damage)\n";
            }
        }
        loc.treasureNames.clear();
        loc.treasureValues.clear();
    }

    void showStats() {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║          CHARACTER STATS               ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "Name:     " << playerName_ << "\n";
        std::cout << "Level:    " << playerLevel_ << "\n";
        std::cout << "Health:   " << playerHealth_ << "/" << playerMaxHealth_ << "\n";
        std::cout << "Attack:   " << playerAttack_;
        if (equippedWeapon_)
            std::cout << " + " << equippedWeapon_->getDamage() << " (weapon)";
        std::cout << "\n";
        std::cout << "Gold:     " << playerGold_ << "\n";
        std::cout << "Location: " << currentLocationName_ << "\n";

        int barWidth = 20;
        int filled = playerMaxHealth_ > 0 ? (playerHealth_ * barWidth) / playerMaxHealth_ : 0;
        std::cout << "HP:       [";
        for (int i = 0; i < barWidth; i++)
            std::cout << (i < filled ? "█" : "░");
        std::cout << "]\n";
    }

    // ALGORITHM OPPORTUNITY: the visited-room count could use
    // std::count_if(dungeon_.begin(), dungeon_.end(),
    //     [](const auto& loc) { return loc->visited; });

    void showQuests() {
        std::cout << "\n📜 Quest Log:\n";
        std::cout << "═══════════════════════════════════\n";

        auto active = questManager_->getActiveQuests();
        auto completed = questManager_->getCompletedQuests();

        if (!active.empty()) {
            std::cout << "\n🔸 Active Quests:\n";
            // ALGORITHM OPPORTUNITY: std::for_each
            for (const auto& quest : active)
                std::cout << "   [ ] " << quest.name << "\n";
        }

        if (!completed.empty()) {
            std::cout << "\n✅ Completed Quests:\n";
            for (const auto& quest : completed)
                std::cout << "   [✓] " << quest.name << "\n";
        }

        std::cout << "\nProgress: " << questManager_->getCompletedCount() << "/"
                  << questManager_->getQuestCount() << " quests completed\n";
    }
};
