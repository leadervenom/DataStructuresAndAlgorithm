#ifndef PRIORITY_H
#define PRIORITY_H

#include <string>
#include <unordered_map>
#include <vector>

#include "player.h"

// Load priority CSV into a map of playerId -> missCount.
std::unordered_map<int, int> loadPriority(const std::string& path);
// Write priority map to CSV (playerId, missCount).
void savePriority(const std::string& path, const std::unordered_map<int, int>& priority);
// Increment misses for non-selected players, reset for selected players.
void updatePriorityForRound(const std::vector<Player>& allPlayers,
                            const std::vector<int>& selectedIds,
                            std::unordered_map<int, int>& priority);
// Read a player's current miss count (0 if missing).
int getPriority(const std::unordered_map<int, int>& priority, int playerId);

#endif
