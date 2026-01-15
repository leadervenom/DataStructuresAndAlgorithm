#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ban_system.h"
#include "hero.h"
#include "matchmaking.h"
#include "priority.h"
#include "player.h"

using namespace std;

// Parse a single role token like "R" or "Mid" into Role enum.
bool parseRoleToken(const string& token, Role& out) {
    for (char c : token) {
        if (!isspace(static_cast<unsigned char>(c))) {
            return roleFromChar(c, out);
        }
    }
    return false;
}

// Parse CSV line into a Player record.
// Expected formats:
//   id,mmr,behavior
//   id,mmr,behavior,role
bool parsePlayerLine(const string& line, Player& out) {
    // Empty line = invalid.
    if (line.empty()) {
        return false;
    }

    // Use a stringstream to split by commas.
    stringstream ss(line);
    string token;

    // 1) Parse id.
    if (!getline(ss, token, ',')) return false;
    try {
        out.id = stoi(token);
    } catch (...) {
        return false; // Non-numeric id.
    }

    // 2) Parse mmr.
    if (!getline(ss, token, ',')) return false;
    try {
        out.mmr = stoi(token);
    } catch (...) {
        return false; // Non-numeric mmr.
    }

    // 3) Parse behavior.
    if (!getline(ss, token, ',')) return false;
    try {
        out.behavior = stoi(token);
    } catch (...) {
        return false; // Non-numeric behavior.
    }

    // 4) Optional role. If missing, assign based on id.
    if (!getline(ss, token, ',')) {
        out.role = roleFromIndex(out.id);
        return true;
    }

    // If role token exists, parse it into a Role enum.
    if (!parseRoleToken(token, out.role)) return false;

    return true;
}

// Load all players from CSV file into a vector.
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

// Print a team roster with key stats.
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

// Entry point: load players, build teams, run ban phase.
int main() {

    //variable database is crated to load players from the csv file into a searchable vector
    vector<Player> database = loadPlayersFromCsv("players.csv");
    if (database.empty()) {
        cout << "Failed to load players.csv or no players found.\n";
        return 0;
    }

    //creates empty queue to store Player objects  it represents the players waiting for matchmaking   
    //loops over all players in database /// the auto& is basically a function that says find the type for me
    //basically for every player in database add current player to the back of the queue(ENQUEUING)
    queue<Player> matchmakingQueue;
    for (const auto& p : database) { 
        matchmakingQueue.push(p); 
    }

    //matchmaking is assumed to be no ,so start is initialized as n
    // if answer is not equals to y or Y then cancel or not suitable as it would continue regardless of one of them being true so && is suitable
    //the auto function is better to use so that if the Player type changes type it does not become a concern
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

    //converst players in queue to vector why because queue cant be searched or accessed from the middle so only the back or the front is accessible
    //objects must be initialized so user(0,0,0,Role::Roam) is used as a dummy starter object but is overwritten
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

    //for user to choose the role they desire
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

    // File path where skip counts are stored between runs.
    const string priorityPath = "priority.csv";
    // Load current priority (miss counts) or start empty if file is missing.
    unordered_map<int, int> priority = loadPriority(priorityPath);

    // Build teams using priority to favor players skipped in previous rounds.
    if (!buildTeams(matchmakingQueue, user, teamA, teamB, priority, error)) {
        cout << error << "\n";
        return 0;
    }

    // Show the final team rosters.
    cout << "\nTEAM A:\n";
    printTeam("Team A", teamA);
    cout << "TEAM B:\n";
    printTeam("Team B", teamB);

    // Gather selected ids to update priority after this round.
    vector<int> selectedIds;
    selectedIds.reserve(teamA.size() + teamB.size());
    for (const auto& p : teamA) {
        selectedIds.push_back(p.id);
    }
    for (const auto& p : teamB) {
        selectedIds.push_back(p.id);
    }
    // Increase misses for non-selected players, reset selected players to 0.
    updatePriorityForRound(allPlayers, selectedIds, priority);
    // Save updated priority for the next run.
    savePriority(priorityPath, priority);

    // Load heroes to start the ban phase.
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
