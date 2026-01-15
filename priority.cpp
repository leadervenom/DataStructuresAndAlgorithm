#include "priority.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_set>

// Parse CSV lines into a map of playerId -> missCount.
std::unordered_map<int, int> loadPriority(const std::string& path) {
    std::unordered_map<int, int> priority;
    std::ifstream file(path);
    if (!file) {
        return priority;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip blank lines and malformed rows.
        if (line.empty()) {
            continue;
        }
        std::stringstream ss(line);
        std::string token;
        if (!std::getline(ss, token, ',')) {
            continue;
        }
        int id = 0;
        try {
            id = std::stoi(token);
        } catch (...) {
            continue;
        }
        if (!std::getline(ss, token, ',')) {
            continue;
        }
        int misses = 0;
        try {
            misses = std::stoi(token);
        } catch (...) {
            continue;
        }
        if (misses < 0) {
            misses = 0;
        }
        // Store the last observed count for each id.
        priority[id] = misses;
    }

    return priority;
}

// Persist the priority map as sorted CSV for easy inspection.
void savePriority(const std::string& path, const std::unordered_map<int, int>& priority) {
    std::vector<std::pair<int, int>> rows;
    rows.reserve(priority.size());
    for (const auto& entry : priority) {
        rows.push_back(entry);
    }
    // Sort by player id to keep file stable.
    std::sort(rows.begin(), rows.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    std::ofstream file(path, std::ios::trunc);
    if (!file) {
        return;
    }
    for (const auto& row : rows) {
        // Write "id,misses" per line.
        file << row.first << "," << row.second << "\n";
    }
}

// Update misses: selected players reset to 0, others increment.
void updatePriorityForRound(const std::vector<Player>& allPlayers,
                            const std::vector<int>& selectedIds,
                            std::unordered_map<int, int>& priority) {
    std::unordered_set<int> selected(selectedIds.begin(), selectedIds.end());
    for (const auto& p : allPlayers) {
        int current = 0;
        auto it = priority.find(p.id);
        if (it != priority.end()) {
            current = it->second;
        }
        if (selected.find(p.id) != selected.end()) {
            // Selected players reset their miss count.
            priority[p.id] = 0;
        } else {
            // Everyone else increments their miss count.
            priority[p.id] = current + 1;
        }
    }
}

// Return miss count for a player (0 if not present).
int getPriority(const std::unordered_map<int, int>& priority, int playerId) {
    auto it = priority.find(playerId);
    if (it == priority.end()) {
        // Default to 0 if the player isn't tracked yet.
        return 0;
    }
    return it->second;
}
