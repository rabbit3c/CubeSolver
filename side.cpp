#include "side.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Side::Side(color c) {
    side.fill(c);
}

Side::Side(color colors[9]) {
    for (int i = 0; i < side.size(); i++) {
        side[i] = colors[i];
    }
}

const int Side::rotations[3][9] = {
    {6,3,0, 7,4,1, 8,5,2},
    {8,7,6, 5,4,3, 2,1,0},
    {2,5,8, 1,4,7, 0,3,6}
};

//Clockwise Rotation
void Side::rotate(int n) {
    if (n > 3 || n < 1) throw invalid_argument("Number of rotation must be between 1 and 3");
    array<color, 9> temp;

    for (int i = 0; i < side.size(); i++) {
        temp[i] = side[rotations[n - 1][i]];
    }
    side = temp;
}

array<color, 3> Side::reverse(array<color, 3> line) {
    return {line[2], line[1], line[0]};
}

array<color, 3> Side::getLine(int n) {
    array<color, 3> line;

    for (int i = 0; i < line.size(); i++) {
        line[i] = side[n * 3 + i];
    }

    if (reverseLine) return reverse(line);
    return line;
}

array<color, 3> Side::getCol(int n) {
    array<color, 3> col;

    for (int i = 0; i < col.size(); i++) {
        col[i] = side[i * 3 + n];
    }
    if (reverseCol) return reverse(col);
    return col;
}

void Side::replaceLine(int n, array<color, 3> line) {
    if (reverseLine) line = reverse(line);
    for (int i = 0; i < line.size(); i++) {
        side[n * 3 + i] = line[i];
    }
}

void Side::replaceCol(int n, array<color, 3> col) {
    if (reverseCol) col = reverse(col);
    for (int i = 0; i < col.size(); i++) {
        side[i * 3 + n] = col[i];
    }
}

void Side::print() {
    string stringSide = "";
    for (int i = 0; i < side.size(); i++) {
        stringSide.append(colorToString(side[i]));
        if ((i + 1) % 3 == 0) stringSide.append("\n");
    }

    cout << stringSide << endl;
}

bool Side::completion() {
    color c = side[0];
    for (int i = 1; i < side.size(); i++) {
        if (c != side[i]) return false;
    }

    return true;
}