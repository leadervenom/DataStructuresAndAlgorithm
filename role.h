#ifndef ROLE_H
#define ROLE_H

#include <string>

enum class Role {
    Roam,
    Jungler,
    Exp,
    Gold,
    Mid
};

bool roleFromChar(char c, Role& out);
std::string roleLabel(Role r);
char roleCode(Role r);
Role roleFromIndex(int index);

#endif
