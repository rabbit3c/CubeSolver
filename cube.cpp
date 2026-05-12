#include "cube.h"
#include <array>

Cube::Cube() {
    for (int i = 0; i < cube.size(); i++) {
        cube[i] = static_cast<color>(i);
    }
}

void Cube::move(int face, int sides[4], int nLines[4], int n) {
    cube[face].rotate(n);

    array<int, 4> lines;
    for (int i = 0; i < 4; i++) {
        lines[i] = cube[sides[i]].getLine(nLines[i]);
    }

    for (int i = 0; i < 4; i++) {
        int j = (i - n + 4) % 4;

        cube[sides[i]].replaceLine(nLines[i], lines[j]);
    }
}

array<int[4], 6> Cube::sides = {{
    {2, 3, 4, 5},
    {5, 4, 3, 2},
    {1, 2, 0, 4},
    {0, 2, 1, 4},
    {0, 5, 1, 3},
    {5, 0, 3, 1}
}};

array<int[4], 6> Cube::nLines = {{
    {0, 0, 0, 0},
    {2, 2, 2, 2},
    {1, 1, 1, 3},
    {3, 3, 3, 1},
    {2, 3, 0, 1},
    {1, 0, 3, 2}
}};

void Cube::up(int n) {
    move(0, sides[0], nLines[0], n);
    addMove("U", n);
}

void Cube::down(int n) {
    move(1, sides[1], nLines[1], n);
    addMove("D", n);
}

void Cube::right(int n) {
    move(5, sides[2], nLines[2], n);
    addMove("R", n);
}

void Cube::left(int n) {
    move(3, sides[3], nLines[3], n);
    addMove("L", n);
}

void Cube::front(int n) {
    move(2, sides[4], nLines[4], n);
    addMove("F", n);
}

void Cube::back(int n) {
    move(4, sides[5], nLines[5], n);
    addMove("B", n);
}

void Cube::addMove(string c, int n) {
    if (n == 2) c += "2";
    if (n == 3) c += "'";

    moves.push_back(c);
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