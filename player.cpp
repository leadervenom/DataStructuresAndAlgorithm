#include "player.h"

Category mmrCategory(int mmr) {
    if (mmr <= 399) return Category::VeryBad;
    if (mmr <= 799) return Category::Bad;
    if (mmr <= 1199) return Category::Medium;
    if (mmr <= 1599) return Category::Good;
    return Category::VeryGood;
}

Category behaviorCategory(int behavior) {
    if (behavior <= 20) return Category::VeryBad;
    if (behavior <= 40) return Category::Bad;
    if (behavior <= 60) return Category::Medium;
    if (behavior <= 80) return Category::Good;
    return Category::VeryGood;
}

std::string categoryLabel(Category c) {
    switch (c) {
        case Category::VeryBad: return "Very Bad";
        case Category::Bad: return "Bad";
        case Category::Medium: return "Medium";
        case Category::Good: return "Good";
        case Category::VeryGood: return "Very Good";
    }
    return "Unknown";
}
