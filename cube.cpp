#include "cube.h"
#include "symmetry.h"
#include "moves.h"
#include "corners.h"
#include "edges.h"
#include <cmath>

Cube::Cube() {
    corners = solvedCorners;
    edges = solvedEdges;
}

void Cube::move(int face, int n) {
    corners ^= Moves::getCornerXOR(corners, face, n);
    edges ^= Moves::getEdgeXOR(edges, face, n);

    addMove(face, n);
    g++;
}

void Cube::addMove(int i, int n) {
    uint8_t move = i * 10 + n;

    moves.push_back(move);
}

int Cube::lastMove() {
    if (moves.empty()) return -1;
    return moves.back() / 10;
}

void Cube::print() {
    cout << "Cube: " << endl;
    //TODO: Add print Function
}

void Cube::printMoves() {
    string stringI[6] = { "U", "D", "F", "L", "B", "R" };
    string stringN[4] = { "", "", "2", "\'" };

    for (uint8_t move : moves) {
        int i = move / 10;
        int n = move % 10;

        string str = "";
        str.append(stringI[i]);
        str.append(stringN[n]);

        cout << str << " ";
    }
    cout << endl << endl;
}

void Cube::clearMoves() {
    moves.clear();
}

bool Cube::completion() {
    return corners == solvedCorners && edges == solvedEdges;
}

CubeKey Cube::toKey() {
    uint64_t standardizedCorners = Symmetry::standardizeCorners(corners);
    uint64_t standardizedEdges = Symmetry::standardizeEdges(edges, 8);
    return CubeKey{ standardizedCorners, standardizedEdges };
}