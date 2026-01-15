#include "role.h"

bool roleFromChar(char c, Role& out) {
    // Map a single character to a role enum.
    switch (c) {
        case 'R':
        case 'r':
            out = Role::Roam;
            return true;
        case 'J':
        case 'j':
            out = Role::Jungler;
            return true;
        case 'E':
        case 'e':
            out = Role::Exp;
            return true;
        case 'G':
        case 'g':
            out = Role::Gold;
            return true;
        case 'M':
        case 'm':
            out = Role::Mid;
            return true;
        default:
            return false;
    }
}

std::string roleLabel(Role r) {
    // Convert role enum to a display label.
    switch (r) {
        case Role::Roam: return "Roam";
        case Role::Jungler: return "Jungler";
        case Role::Exp: return "Exp";
        case Role::Gold: return "Gold";
        case Role::Mid: return "Mid";
    }
    // Fallback for unexpected values.
    return "Unknown";
}

char roleCode(Role r) {
    // Convert role enum to a single-character code.
    switch (r) {
        case Role::Roam: return 'R';
        case Role::Jungler: return 'J';
        case Role::Exp: return 'E';
        case Role::Gold: return 'G';
        case Role::Mid: return 'M';
    }
    // Fallback character when role is unknown.
    return '?';
}

Role roleFromIndex(int index) {
    int value = index % 5;
    // Normalize negative values into the 0-4 range.
    if (value < 0) value += 5;
    switch (value) {
        case 0: return Role::Roam;
        case 1: return Role::Jungler;
        case 2: return Role::Exp;
        case 3: return Role::Gold;
        case 4: return Role::Mid;
    }
    // Default to Roam for unexpected indices.
    return Role::Roam;
}
