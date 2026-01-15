#include "ban_system.h"

#include <cctype>
#include <iostream>

static std::string toLower(const std::string& s) {
    std::string out;
    // Reserve to avoid reallocations during push_back.
    out.reserve(s.size());
    for (char c : s) {
        // Lowercase using unsigned char to avoid UB.
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return out;
}

static bool pickBan(const std::string& teamName,
                    int banNumber,
                    std::vector<Hero>& pool,
                    std::vector<Hero>& bans) {
    while (true) {
        // Prompt until a valid hero is chosen.
        std::cout << teamName << " ban #" << banNumber << " (enter hero name): ";
        std::string choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }

        // Search the pool for a case-insensitive match.
        std::string wanted = toLower(choice);
        bool found = false;
        for (size_t i = 0; i < pool.size(); ++i) {
            if (toLower(pool[i].name) == wanted) {
                // Move the hero to the banned list.
                bans.push_back(pool[i]);
                pool.erase(pool.begin() + i);
                found = true;
                break;
            }
        }

        if (found) {
            return true;
        }

        std::cout << "Hero not found or already banned. Try again.\n";
    }
}

bool runBanPhase(const std::vector<Hero>& heroes,
                 std::vector<Hero>& bannedA,
                 std::vector<Hero>& bannedB,
                 std::string& error) {
    // Need at least 8 heroes for 4 bans each team.
    if (heroes.size() < 8) {
        error = "Not enough heroes to ban (need at least 8).";
        return false;
    }

    // Copy into a mutable pool that shrinks on each ban.
    std::vector<Hero> pool = heroes;
    for (int i = 1; i <= 4; ++i) {
        if (!pickBan("Team A", i, pool, bannedA)) {
            error = "Failed during Team A ban phase.";
            return false;
        }
    }

    for (int i = 1; i <= 4; ++i) {
        if (!pickBan("Team B", i, pool, bannedB)) {
            error = "Failed during Team B ban phase.";
            return false;
        }
    }

    // Both teams completed all bans.
    return true;
}

void printBans(const std::string& teamName, const std::vector<Hero>& bans) {
    // Print each banned hero with optional role.
    std::cout << teamName << " bans:\n";
    for (const auto& hero : bans) {
        std::cout << "- " << hero.name;
        if (!hero.role.empty()) {
            std::cout << " (" << hero.role << ")";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
