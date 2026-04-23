#pragma once

// ============================================================================
// stl_game.h — Session 11: STL Algorithms
//
// Implement Quest, QuestManager, and the algorithm free functions below.
// Once complete, this file can be copied into
//   sessions/11_standard_library/starter/stl_game.h
// and your code will plug directly into game_world.
// ============================================================================

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
#include<list>

// ============================================================================
// Quest + QuestManager
//
// The GameEngine in game.h calls these with quest IDs:
//   "goblin", "skeleton", "treasure", "dragon"
// ============================================================================

struct Quest {
    std::string id;
    std::string name;
    bool completed;
};

class QuestManager {
   private:
    std::map<std::string, Quest> quests_;
    std::set<std::string> completedQuests_;
	 std::list<int> questRewards;
   public:
    void addQuest(const Quest& quest) {
        quests_[quest.id] = quest;

    }
	 //VIZSGAN LEHET!!! const eseten, lehet hatekonyabb ha specialisabb megvalositast hasznalunk
	 bool isQuestTaken (const Quest& quest) const {
		  
		  return quests_.end() != quests_.find(quest.id);
	 }

    void completeQuest(const std::string& /*questId*/) {
        // TODO: find quest in map, set completed = true, insert id into set
    }

    bool isCompleted(const std::string& /*questId*/) const {
        // TODO: return completedQuests_.count(questId) > 0;
        return false;
    }

    std::vector<Quest> getActiveQuests() const {
        // TODO: iterate quests_, collect those where completed == false
        return {};
    }

    std::vector<Quest> getCompletedQuests() const {
        // TODO: iterate quests_, collect those where completed == true
        return {};
    }

    size_t getQuestCount() const {
        // TODO: return quests_.size();
        return 0;
    }

    size_t getCompletedCount() const {
        // TODO: return completedQuests_.size();
        return 0;
    }
};

// ============================================================================
// Algorithm free functions
//
// These don't plug into the GameEngine directly — they demonstrate
// STL algorithms on game data. The tests exercise them.
// ============================================================================

// A lightweight player record for algorithm exercises.
// (The GameEngine stores player data as flat fields; this struct
// lets us practice algorithms on collections of players.)
struct PlayerRecord {
    std::string name;
    int health;
    std::vector<std::string> inventory;

    PlayerRecord(const std::string& n, int h) : name(n), health(h) {}

    void addItem(const std::string& item) { inventory.push_back(item); }

    bool hasItem(const std::string& item) const {
        // TODO: use std::find instead of this manual loop
        for (const auto& i : inventory) {
            if (i == item) return true;
        }
        return false;
    }
};

// TODO: Sort players by health (ascending) using std::sort + lambda
inline void sortByHealth(std::vector<PlayerRecord>& /*players*/) {
}

// TODO: Find player by name using std::find_if + lambda
//   Return pointer to element, or nullptr
inline PlayerRecord* findByName(std::vector<PlayerRecord>& /*players*/,
                                const std::string& /*name*/) {
    return nullptr;
}

// TODO: Remove all players with health <= 0 using erase-remove idiom
inline void removeDead(std::vector<PlayerRecord>& /*players*/) {
}

// TODO: Count players that have a specific item using std::count_if + lambda
inline int countWithItem(const std::vector<PlayerRecord>& /*players*/,
                         const std::string& /*item*/) {
    return 0;
}

// TODO: Check if ALL players are alive using std::all_of
inline bool allAlive(const std::vector<PlayerRecord>& /*players*/) {
    return false;
}

// TODO: Check if ANY player has a specific item using std::any_of
inline bool anyHasItem(const std::vector<PlayerRecord>& /*players*/,
                       const std::string& /*item*/) {
    return false;
}
