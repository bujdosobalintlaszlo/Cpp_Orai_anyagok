#include "../stl_game.h"

#include <catch2/catch_all.hpp>

// ============================================================================
// Part 1: QuestManager (map + set)
// ============================================================================

TEST_CASE("Quest struct", "[quest]") {
    Quest q{"goblin", "Defeat the Goblin Scout", false};
    REQUIRE(q.id == "goblin");
    REQUIRE(q.name == "Defeat the Goblin Scout");
    REQUIRE_FALSE(q.completed);
}

TEST_CASE("QuestManager add and count", "[quest][map]") {
    QuestManager qm;
    qm.addQuest({"goblin", "Defeat the Goblin Scout", false});
    qm.addQuest({"skeleton", "Defeat the Skeleton Warrior", false});
    REQUIRE(qm.getQuestCount() == 2);
    REQUIRE(qm.getCompletedCount() == 0);
}

TEST_CASE("QuestManager complete", "[quest][set]") {
    QuestManager qm;
    qm.addQuest({"goblin", "Defeat the Goblin Scout", false});
    qm.addQuest({"skeleton", "Defeat the Skeleton Warrior", false});
    qm.completeQuest("goblin");

    REQUIRE(qm.isCompleted("goblin"));
    REQUIRE_FALSE(qm.isCompleted("skeleton"));
    REQUIRE(qm.getCompletedCount() == 1);
}

TEST_CASE("QuestManager active and completed lists", "[quest]") {
    QuestManager qm;
    qm.addQuest({"goblin", "Defeat the Goblin Scout", false});
    qm.addQuest({"skeleton", "Defeat the Skeleton Warrior", false});
    qm.addQuest({"treasure", "Find the Magic Amulet", false});
    qm.completeQuest("skeleton");

    REQUIRE(qm.getActiveQuests().size() == 2);
    REQUIRE(qm.getCompletedQuests().size() == 1);
    REQUIRE(qm.getCompletedQuests()[0].id == "skeleton");
}

TEST_CASE("QuestManager complete all four game quests", "[quest]") {
    QuestManager qm;
    qm.addQuest({"goblin", "Defeat the Goblin Scout", false});
    qm.addQuest({"skeleton", "Defeat the Skeleton Warrior", false});
    qm.addQuest({"treasure", "Find the Magic Amulet", false});
    qm.addQuest({"dragon", "Slay the Ancient Dragon", false});

    qm.completeQuest("goblin");
    qm.completeQuest("skeleton");
    qm.completeQuest("treasure");
    qm.completeQuest("dragon");

    REQUIRE(qm.getActiveQuests().empty());
    REQUIRE(qm.getCompletedCount() == 4);
}

TEST_CASE("QuestManager empty", "[quest][edge]") {
    QuestManager qm;
    REQUIRE(qm.getQuestCount() == 0);
    REQUIRE(qm.getActiveQuests().empty());
    REQUIRE_FALSE(qm.isCompleted("anything"));
}

// ============================================================================
// Part 2: Algorithm free functions on PlayerRecord
// ============================================================================

TEST_CASE("sortByHealth", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 100);
    players.emplace_back("Charlie", 75);

    sortByHealth(players);

    REQUIRE(players[0].name == "Alice");
    REQUIRE(players[1].name == "Charlie");
    REQUIRE(players[2].name == "Bob");
}

TEST_CASE("findByName found", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 100);

    PlayerRecord* p = findByName(players, "Bob");
    REQUIRE(p != nullptr);
    REQUIRE(p->name == "Bob");
    REQUIRE(p->health == 100);
}

TEST_CASE("findByName not found", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);

    REQUIRE(findByName(players, "Nobody") == nullptr);
}

TEST_CASE("removeDead", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 0);
    players.emplace_back("Charlie", 75);
    players.emplace_back("Dave", -10);

    removeDead(players);

    REQUIRE(players.size() == 2);
    REQUIRE(players[0].name == "Alice");
    REQUIRE(players[1].name == "Charlie");
}

TEST_CASE("removeDead empty", "[algorithm][edge]") {
    std::vector<PlayerRecord> players;
    removeDead(players);
    REQUIRE(players.empty());
}

TEST_CASE("countWithItem", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 100);
    players.emplace_back("Charlie", 75);

    players[0].addItem("Sword");
    players[1].addItem("Sword");
    players[2].addItem("Axe");

    REQUIRE(countWithItem(players, "Sword") == 2);
    REQUIRE(countWithItem(players, "Axe") == 1);
    REQUIRE(countWithItem(players, "Bow") == 0);
}

TEST_CASE("allAlive", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 100);

    REQUIRE(allAlive(players));

    players.emplace_back("Charlie", 0);
    REQUIRE_FALSE(allAlive(players));
}

TEST_CASE("allAlive empty", "[algorithm][edge]") {
    std::vector<PlayerRecord> players;
    REQUIRE(allAlive(players));  // vacuously true
}

TEST_CASE("anyHasItem", "[algorithm]") {
    std::vector<PlayerRecord> players;
    players.emplace_back("Alice", 50);
    players.emplace_back("Bob", 100);

    players[0].addItem("Sword");

    REQUIRE(anyHasItem(players, "Sword"));
    REQUIRE_FALSE(anyHasItem(players, "Shield"));
}

// ============================================================================
// Integration
// ============================================================================

TEST_CASE("Full party management scenario", "[integration]") {
    std::vector<PlayerRecord> party;
    party.emplace_back("Alice", 100);
    party.emplace_back("Bob", 50);
    party.emplace_back("Charlie", 0);
    party.emplace_back("Dave", 75);

    party[0].addItem("Sword");
    party[1].addItem("Sword");
    party[3].addItem("Axe");

    removeDead(party);
    REQUIRE(party.size() == 3);

    sortByHealth(party);
    REQUIRE(party[0].name == "Bob");
    REQUIRE(party[1].name == "Dave");
    REQUIRE(party[2].name == "Alice");

    REQUIRE(countWithItem(party, "Sword") == 2);
    REQUIRE(allAlive(party));
    REQUIRE(anyHasItem(party, "Axe"));
    REQUIRE_FALSE(anyHasItem(party, "Shield"));
}
