#include "matchmaking.h"
#include "role_assignment.h"

#include <algorithm>
#include <cmath>

// Consume queue into a vector for easier access.
std::vector<Player> queueToVector(std::queue<Player> q) {
    std::vector<Player> out;
    while (!q.empty()) {
        out.push_back(q.front());
        q.pop();
    }
    return out;
}

// Select players in user's MMR category and sort by closeness.
std::vector<Player> findSameMmrCategorySorted(const std::vector<Player>& all, const Player& user) {
    std::vector<Player> result;
    Category userMmr = mmrCategory(user.mmr);
    for (const auto& p : all) {
        if (p.id == user.id) continue;
        if (mmrCategory(p.mmr) == userMmr) {
            result.push_back(p);
        }
    }
    std::sort(result.begin(), result.end(), [&](const Player& a, const Player& b) {
        if (a.mmr != b.mmr) return a.mmr > b.mmr;
        int da = std::abs(a.mmr - user.mmr);
        int db = std::abs(b.mmr - user.mmr);
        if (da != db) return da < db;
        return a.id < b.id;
    });
    return result;
}

// Further filter candidates to match behavior category.
std::vector<Player> findSameMmrAndBehavior(const std::vector<Player>& mmrSorted, const Player& user) {
    std::vector<Player> result;
    Category userMmr = mmrCategory(user.mmr);
    Category userBehavior = behaviorCategory(user.behavior);
    for (const auto& p : mmrSorted) {
        if (mmrCategory(p.mmr) == userMmr && behaviorCategory(p.behavior) == userBehavior) {
            result.push_back(p);
        }
    }
    return result;
}

// Remove chosen ids from queue and append to output vector.
void dequeuePlayers(std::queue<Player>& q, const std::vector<int>& ids, std::vector<Player>& out) {
    int sz = static_cast<int>(q.size());
    for (int i = 0; i < sz; ++i) {
        Player p = q.front();
        q.pop();
        bool selected = false;
        for (int id : ids) {
            if (p.id == id) {
                selected = true;
                break;
            }
        }
        if (selected) {
            out.push_back(p);
        } else {
            q.push(p);
        }
    }
}

// Build teams by MMR/behavior filter, then role assignment.
bool buildTeams(std::queue<Player>& matchmakingQueue,
                const Player& user,
                std::vector<Player>& teamA,
                std::vector<Player>& teamB,
                const std::unordered_map<int, int>& priority,
                std::string& error) {
    std::vector<Player> allPlayers = queueToVector(matchmakingQueue);
    std::vector<Player> mmrSorted = findSameMmrCategorySorted(allPlayers, user);
    std::vector<Player> sameBoth = findSameMmrAndBehavior(mmrSorted, user);

    if (sameBoth.size() < 9) {
        error = "Not enough matching players for 5v5 in the same MMR and behavior category.";
        return false;
    }

    std::vector<int> teamAIds;
    std::vector<int> teamBIds;
    teamAIds.push_back(user.id);

    if (!pickRoleTeams(sameBoth, user, teamAIds, teamBIds, priority, error)) {
        return false;
    }

    dequeuePlayers(matchmakingQueue, teamAIds, teamA);
    dequeuePlayers(matchmakingQueue, teamBIds, teamB);

    if (teamA.size() != 5 || teamB.size() != 5) {
        error = "Failed to dequeue full 5v5 teams from the queue.";
        return false;
    }

    return true;
}
