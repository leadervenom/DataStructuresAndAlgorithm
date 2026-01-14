#ifndef BAN_SYSTEM_H
#define BAN_SYSTEM_H

#include <string>
#include <vector>

#include "hero.h"

bool runBanPhase(const std::vector<Hero>& heroes,
                 std::vector<Hero>& bannedA,
                 std::vector<Hero>& bannedB,
                 std::string& error);

void printBans(const std::string& teamName, const std::vector<Hero>& bans);

#endif
