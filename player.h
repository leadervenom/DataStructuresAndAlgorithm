#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "role.h"

struct Player {
    int id;
    int mmr;
    int behavior;
    Role role;
};

enum class Category {
    VeryBad,
    Bad,
    Medium,
    Good,
    VeryGood
};

Category mmrCategory(int mmr);
Category behaviorCategory(int behavior);
std::string categoryLabel(Category c);

#endif
