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

// Buckets for MMR/behavior matching.
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
