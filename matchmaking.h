#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "player.h"

// Snapshot a queue into a vector for searching/sorting.
std::vector<Player> queueToVector(std::queue<Player> q);
// Find players in the same MMR category, sorted by closeness.
std::vector<Player> findSameMmrCategorySorted(const std::vector<Player>& all, const Player& user);
// Filter players to match both MMR and behavior category.
std::vector<Player> findSameMmrAndBehavior(const std::vector<Player>& mmrSorted, const Player& user);
// Remove players by id from queue and push into output list.
void dequeuePlayers(std::queue<Player>& q, const std::vector<int>& ids, std::vector<Player>& out);

// Orchestrate filtering and role assignment to build 5v5 teams.
bool buildTeams(
    std::queue<Player>& matchmakingQueue,
                const Player& user,
                std::vector<Player>& teamA,
                std::vector<Player>& teamB,
                const std::unordered_map<int, int>& priority,
                std::string& error);

#endif
