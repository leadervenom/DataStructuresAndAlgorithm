#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <queue>
#include <string>
#include <vector>

#include "player.h"

std::vector<Player> queueToVector(std::queue<Player> q);
std::vector<Player> findSameMmrCategorySorted(const std::vector<Player>& all, const Player& user);
std::vector<Player> findSameMmrAndBehavior(const std::vector<Player>& mmrSorted, const Player& user);
void dequeuePlayers(std::queue<Player>& q, const std::vector<int>& ids, std::vector<Player>& out);

bool buildTeams(
    std::queue<Player>& matchmakingQueue,
                const Player& user,
                std::vector<Player>& teamA,
                std::vector<Player>& teamB,
                std::string& error);

#endif
