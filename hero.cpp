#include "hero.h"

#include <fstream>
#include <sstream>

static bool parseHeroLine(const std::string& line, Hero& out) {
    if (line.empty()) {
        return false;
    }

    std::stringstream ss(line);
    std::string token;
    if (!std::getline(ss, token, ',')) return false;
    out.name = token;

    if (std::getline(ss, token, ',')) {
        out.role = token;
    } else {
        out.role.clear();
    }

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
        heroes.push_back(first);
    }

    while (std::getline(file, line)) {
        Hero hero;
        if (parseHeroLine(line, hero)) {
            heroes.push_back(hero);
        }
    }

    return heroes;
}
