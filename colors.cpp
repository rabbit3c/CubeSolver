#include "colors.h"

using namespace std;

string colorToString(color c) {
    switch (c) {
        case WHITE:
            return "⬜";
        case YELLOW:
            return "🟨";
        case BLUE:
            return "🟦";
        case ORANGE:
            return "🟧";
        case GREEN:
            return "🟩";
        case RED:
            return "🟥";
        default:
            throw;
    }
}