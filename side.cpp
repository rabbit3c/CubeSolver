#include "side.h"
#include <iostream>

using namespace std;

Side::Side(color c) {
    side.fill({c, c, c});
}

Side::Side(color colors[9]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            side[i][j] = colors[i * 3 + j];
        }
    }
}

void Side::print() {
    string stringSide = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            stringSide.append(colorToString(side[i][j]));
        }
        stringSide.append("\n");
    }

    cout << stringSide << endl;
}

bool Side::completion() {
    color c = side[0][0];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (c != side[i][j]) return false;
        }
    }

    return true;
}