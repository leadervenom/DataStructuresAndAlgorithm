#ifndef PRIORITY_H
#define PRIORITY_H

#include <string>
#include <unordered_map>
#include <vector>

#include "player.h"


//constructors for the feature
std::unordered_map<int, int> loadPriority(const std::string& path);
void savePriority(const std::string& path, const std::unordered_map<int, int>& priority);
void updatePriorityForRound(const std::vector<Player>& allPlayers,
                            const std::vector<int>& selectedIds,
                            std::unordered_map<int, int>& priority);
int getPriority(const std::unordered_map<int, int>& priority, int playerId);

#endif
