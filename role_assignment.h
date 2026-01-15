#ifndef ROLE_ASSIGNMENT_H
#define ROLE_ASSIGNMENT_H

#include <string>
#include <unordered_map>
#include <vector>

#include "player.h"

// Build 5v5 teams by role using priority to break ties.
bool pickRoleTeams(const std::vector<Player>& candidates,
                   const Player& user,
                   std::vector<int>& teamAIds,
                   std::vector<int>& teamBIds,
                   const std::unordered_map<int, int>& priority,
                   std::string& error);

#endif
