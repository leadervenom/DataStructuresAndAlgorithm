#include "role_assignment.h"

#include <unordered_set>

static std::vector<Role> allRoles() {
    return {Role::Roam, Role::Jungler, Role::Exp, Role::Gold, Role::Mid};
}

static bool pickRole(const std::vector<Player>& candidates,
                     Role role,
                     const std::unordered_set<int>& used,
                     int& outId) {
    for (const auto& p : candidates) {
        if (p.role == role && used.find(p.id) == used.end()) {
            outId = p.id;
            return true;
        }
    }
    return false;
}

bool pickRoleTeams(const std::vector<Player>& candidates,
                   const Player& user,
                   std::vector<int>& teamAIds,
                   std::vector<int>& teamBIds,
                   std::string& error) {
    std::unordered_set<int> used;
    used.insert(user.id);

    std::vector<Role> roles = allRoles();
    for (Role r : roles) {
        if (r == user.role) {
            continue;
        }
        int id = 0;
        if (!pickRole(candidates, r, used, id)) {
            error = "Not enough players for team A role: " + roleLabel(r);
            return false;
        }
        teamAIds.push_back(id);
        used.insert(id);
    }

    for (Role r : roles) {
        int id = 0;
        if (!pickRole(candidates, r, used, id)) {
            error = "Not enough players for team B role: " + roleLabel(r);
            return false;
        }
        teamBIds.push_back(id);
        used.insert(id);
    }

    return true;
}
