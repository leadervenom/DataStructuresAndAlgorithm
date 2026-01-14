#ifndef ROLE_ASSIGNMENT_H
#define ROLE_ASSIGNMENT_H

#include <string>
#include <vector>

#include "player.h"

bool pickRoleTeams(const std::vector<Player>& candidates,
                   const Player& user,
                   std::vector<int>& teamAIds,
                   std::vector<int>& teamBIds,
                   std::string& error);

#endif
