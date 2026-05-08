#include "cube.h"
#include <array>

Cube::Cube() {
    for (int i = 0; i < cube.size(); i++) {
        cube[i] = static_cast<color>(i);
        
        if (i == 4) cube[i].reverseCol = true;
        if (i == 3) cube[i].reverseCol = true;
        if (i == 1) cube[i].reverseLine = true;
    }
}

array<color, 3> reverse(array<color, 3> line) {
    return {line[2], line[1], line[0]};
}

void Cube::move(int face, int sides[4], bool types[4], int nLines[4], int n) {
    cube[face].rotate(n);

    array<array<color, 3>, 4> lines;
    for (int i = 0; i < 4; i++) {
        if (types[i]) lines[i] = cube[sides[i]].getLine(nLines[i]);
        else lines[i] = cube[sides[i]].getCol(nLines[i]);
    }

    for (int i = 0; i < 4; i++) {
        int j = (i - n + 4) % 4;

        if (types[i]) cube[sides[i]].replaceLine(nLines[i], lines[j]);
        else cube[sides[i]].replaceCol(nLines[i], lines[j]);
    }
}

void Cube::addMove(string c, int n) {
    if (n == 2) c += "2";
    if (n == 3) c += "'";

    moves.push_back(c);
}

void Cube::up(int n) {
    int sides[4] = {2, 3, 4, 5};
    int nLines[4] = {0, 0, 0, 0};
    bool types[4] = {true, true, true, true};

    move(0, sides, types, nLines, n);
    addMove("U", n);
}

void Cube::down(int n) {
    int sides[4] = {5, 4, 3, 2};
    int nLines[4] = {2, 2, 2, 2};
    bool types[4] = {true, true, true, true};

    move(1, sides, types, nLines, n);
    addMove("D", n);
}

void Cube::right(int n) {
    int sides[4] = {1, 2, 0, 4};
    int nLines[4] = {2, 2, 2, 0};
    bool types[4] = {false, false, false, false};

    move(5, sides, types, nLines, n);
    addMove("R", n);
}

void Cube::left(int n) {
    int sides[4] = {0, 2, 1, 4};
    int nLines[4] = {0, 0, 0, 2};
    bool types[4] = {false, false, false, false};

    move(3, sides, types, nLines, n);
    addMove("L", n);
}

void Cube::front(int n) {
    int sides[4] = {0, 5, 1, 3};
    int nLines[4] = {2, 0, 0, 2};
    bool types[4] = {true, false, true, false};

    move(2, sides, types, nLines, n);
    addMove("F", n);
}

void Cube::back(int n) {
    int sides[4] = {5, 0, 3, 1};
    int nLines[4] = {2, 0, 0, 2};
    bool types[4] = {false, true, false, true};

    move(4, sides, types, nLines, n);
    addMove("B", n);
}

void Cube::print() {
    cout << "Cube: " << endl;

    for (Side side : cube) {
        side.print();
    }
}

void Cube::printMoves() {
    for (string move : moves) {
        cout << move << " ";
    }
    cout << endl;
}

void Cube::clearMoves() {
    moves.clear();
}

bool Cube::completion() {
    for (Side side : cube) {
        if (!side.completion()) return false;
    }
    
    return true;
}