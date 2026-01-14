#ifndef HERO_H
#define HERO_H

#include <string>
#include <vector>

struct Hero {
    std::string name;
    std::string role;
};

std::vector<Hero> loadHeroesFromCsv(const std::string& path);

#endif
