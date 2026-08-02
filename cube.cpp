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
    uint64_t cornerIDs = corners & Moves::masksCornerIDs[face];
    uint64_t cornerTwists = corners & Moves::masksCornerTwists[face];
    uint64_t edgeIDs = edges & Moves::masksEdgeIDs[face];
    uint64_t edgeTwists = edges & Moves::masksEdgeTwists[face];

    corners &= ~Moves::masksCornerIDs[face] & ~Moves::masksCornerTwists[face];
    edges &= ~Moves::masksEdgeIDs[face] & ~Moves::masksEdgeTwists[face];

    corners |= Moves::cornerIDsLookup[face][n - 1][cornerIDs] | Moves::cornerTwistsLookup[face][n - 1][cornerTwists];
    edges |= Moves::edgeIDsLookup[face][n - 1][edgeIDs] | Moves::edgeTwistsLookup[face][n - 1][edgeTwists];

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
    cout << endl;
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