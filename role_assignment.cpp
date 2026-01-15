#include "role_assignment.h"

#include <unordered_set>

// Ordered list of roles used for team composition.
static std::vector<Role> allRoles() {
    return {Role::Roam, Role::Jungler, Role::Exp, Role::Gold, Role::Mid};
}

// Pick the highest-priority available player for a role.
static bool pickRole(const std::vector<Player>& candidates,
                     Role role,
                     const std::unordered_set<int>& used,
                     const std::unordered_map<int, int>& priority,
                     int& outId) {
    bool found = false;
    int bestPriority = -1;
    for (const auto& p : candidates) {
        if (p.role != role || used.find(p.id) != used.end()) {
            continue;
        }
        int pPriority = 0;
        auto it = priority.find(p.id);
        if (it != priority.end()) {
            pPriority = it->second;
        }
        if (!found || pPriority > bestPriority) {
            bestPriority = pPriority;
            outId = p.id;
            found = true;
        }
    }
    return found;
}

// Assign players to team A then team B ensuring one of each role.
bool pickRoleTeams(const std::vector<Player>& candidates,
                   const Player& user,
                   std::vector<int>& teamAIds,
                   std::vector<int>& teamBIds,
                   const std::unordered_map<int, int>& priority,
                   std::string& error) {
    std::unordered_set<int> used;
    used.insert(user.id);

    std::vector<Role> roles = allRoles();
    for (Role r : roles) {
        if (r == user.role) {
            continue;
        }
        int id = 0;
        if (!pickRole(candidates, r, used, priority, id)) {
            error = "Not enough players for team A role: " + roleLabel(r);
            return false;
        }
        teamAIds.push_back(id);
        used.insert(id);
    }

    for (Role r : roles) {
        int id = 0;
        if (!pickRole(candidates, r, used, priority, id)) {
            error = "Not enough players for team B role: " + roleLabel(r);
            return false;
        }
        teamBIds.push_back(id);
        used.insert(id);
    }

    return true;
}
