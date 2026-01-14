#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "ban_system.h"
#include "hero.h"
#include "matchmaking.h"
#include "player.h"

using namespace std;

bool parseRoleToken(const string& token, Role& out) {
    for (char c : token) {
        if (!isspace(static_cast<unsigned char>(c))) {
            return roleFromChar(c, out);
        }
    }
    return false;
}

bool parsePlayerLine(const string& line, Player& out) {
    if (line.empty()) {
        return false;
    }

    stringstream ss(line);
    string token;
    if (!getline(ss, token, ',')) return false;
    try {
        out.id = stoi(token);
    } catch (...) {
        return false;
    }

    if (!getline(ss, token, ',')) return false;
    try {
        out.mmr = stoi(token);
    } catch (...) {
        return false;
    }

    if (!getline(ss, token, ',')) return false;
    try {
        out.behavior = stoi(token);
    } catch (...) {
        return false;
    }

    if (!getline(ss, token, ',')) {
        out.role = roleFromIndex(out.id);
        return true;
    }
    if (!parseRoleToken(token, out.role)) return false;

    return true;
}

vector<Player> loadPlayersFromCsv(const string& path) {
    vector<Player> players;
    ifstream file(path);
    if (!file) {
        return players;
    }

    string line;
    if (!getline(file, line)) {
        return players;
    }

    Player first;
    if (parsePlayerLine(line, first)) {
        players.push_back(first);
    }

    while (getline(file, line)) {
        Player p;
        if (parsePlayerLine(line, p)) {
            players.push_back(p);
        }
    }

    return players;
}

void printTeam(const string& name, const vector<Player>& team) {
    cout << name << " (" << team.size() << " players)" << "\n";
    for (const auto& p : team) {
        cout << "ID: " << p.id
             << " | MMR: " << p.mmr
             << " (" << categoryLabel(mmrCategory(p.mmr)) << ")"
             << " | Behavior: " << p.behavior
             << " (" << categoryLabel(behaviorCategory(p.behavior)) << ")"
             << " | Role: " << roleLabel(p.role) << "\n";
    }
    cout << "\n";
}

int main() {
    vector<Player> database = loadPlayersFromCsv("players.csv");
    if (database.empty()) {
        cout << "Failed to load players.csv or no players found.\n";
        return 0;
    }

    queue<Player> matchmakingQueue;
    for (const auto& p : database) {
        matchmakingQueue.push(p);
    }

    char start = 'n';
    cout << "Start matchmaking? (y/n): ";
    cin >> start;
    if (start != 'y' && start != 'Y') {
        cout << "Matchmaking cancelled.\n";
        return 0;
    }

    int userId = 0;
    cout << "Enter player ID: ";
    cin >> userId;

    vector<Player> allPlayers = queueToVector(matchmakingQueue);
    Player user{0, 0, 0, Role::Roam};
    bool found = false;
    for (const auto& p : allPlayers) {
        if (p.id == userId) {
            user = p;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Player ID not found in database.\n";
        return 0;
    }

    Role desiredRole;
    char roleInput = '\0';
    while (true) {
        cout << "Choose role [R/r=Roam, J/j=Jungler, E/e=Exp, G/g=Gold, M/m=Mid]: ";
        cin >> roleInput;
        if (roleFromChar(roleInput, desiredRole)) {
            break;
        }
        cout << "Invalid role. Try again.\n";
    }

    // Always override the selected user's role to be dynamic from input.
    user.role = desiredRole;

    // Update the queued copy of the user so output reflects the chosen role.
    int qsize = static_cast<int>(matchmakingQueue.size());
    for (int i = 0; i < qsize; ++i) {
        Player p = matchmakingQueue.front();
        matchmakingQueue.pop();
        if (p.id == user.id) {
            p.role = user.role;
        }
        matchmakingQueue.push(p);
    }

    vector<Player> teamA;
    vector<Player> teamB;
    string error;

    if (!buildTeams(matchmakingQueue, user, teamA, teamB, error)) {
        cout << error << "\n";
        return 0;
    }

    cout << "\nTEAM A:\n";
    printTeam("Team A", teamA);
    cout << "TEAM B:\n";
    printTeam("Team B", teamB);

    vector<Hero> heroes = loadHeroesFromCsv("heroes.csv");
    if (heroes.empty()) {
        cout << "Failed to load heroes.csv or no heroes found.\n";
        return 0;
    }

    vector<Hero> bannedA;
    vector<Hero> bannedB;
    if (!runBanPhase(heroes, bannedA, bannedB, error)) {
        cout << error << "\n";
        return 0;
    }

    cout << "BAN PHASE RESULTS:\n";
    printBans("Team A", bannedA);
    printBans("Team B", bannedB);

    return 0;
}
