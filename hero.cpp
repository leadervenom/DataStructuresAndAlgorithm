#include "hero.h"

#include <fstream>
#include <sstream>

static bool parseHeroLine(const std::string& line, Hero& out) {
    // Reject empty lines early.
    if (line.empty()) {
        return false;
    }

    std::stringstream ss(line);
    std::string token;
    if (!std::getline(ss, token, ',')) return false;
    // First token is the hero name.
    out.name = token;

    if (std::getline(ss, token, ',')) {
        // Optional second token is the role.
        out.role = token;
    } else {
        out.role.clear();
    }

    // Name must be present for a valid record.
    return !out.name.empty();
}

std::vector<Hero> loadHeroesFromCsv(const std::string& path) {
    std::vector<Hero> heroes;
    std::ifstream file(path);
    if (!file) {
        return heroes;
    }

    std::string line;
    if (!std::getline(file, line)) {
        return heroes;
    }

    Hero first;
    if (parseHeroLine(line, first)) {
        // Keep the first line if it is a valid hero row.
        heroes.push_back(first);
    }

    while (std::getline(file, line)) {
        // Parse each remaining line into a Hero record.
        Hero hero;
        if (parseHeroLine(line, hero)) {
            heroes.push_back(hero);
        }
    }

    return heroes;
}
