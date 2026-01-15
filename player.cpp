#include "player.h"

// Map MMR score into a coarse category.
Category mmrCategory(int mmr) {
    // Bucket MMR into broad skill bands.
    if (mmr <= 399) return Category::VeryBad;
    if (mmr <= 799) return Category::Bad;
    if (mmr <= 1199) return Category::Medium;
    if (mmr <= 1599) return Category::Good;
    return Category::VeryGood;
}

// Map behavior score into a coarse category.
Category behaviorCategory(int behavior) {
    // Use simple thresholds to map behavior to a category.
    if (behavior <= 20) return Category::VeryBad;
    if (behavior <= 40) return Category::Bad;
    if (behavior <= 60) return Category::Medium;
    if (behavior <= 80) return Category::Good;
    return Category::VeryGood;
}

// Display label for a category enum.
std::string categoryLabel(Category c) {
    // Convert enum to display text.
    switch (c) {
        case Category::VeryBad: return "Very Bad";
        case Category::Bad: return "Bad";
        case Category::Medium: return "Medium";
        case Category::Good: return "Good";
        case Category::VeryGood: return "Very Good";
    }
    // Fallback for unexpected enum values.
    return "Unknown";
}
