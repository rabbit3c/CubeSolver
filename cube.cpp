#include "cube.h"
#include "symmetry.h"
#include <cmath>

Cube::Cube() {
    corners = solvedCorners;
    edges = solvedEdges;
}

void Cube::move(int face, int n) {
    array<uint64_t, 4> c;
    array<uint64_t, 4> e;

    for (int i = 0; i < 4; i++) {
        uint8_t corner = getCorner(cornersMoves[face][i]);
        uint8_t edge = getEdge(edgesMoves[face][i]);

        if (face > 1 && n != 2) {
            uint8_t twist = (corner >> 3) & maskCornerTwist;
            corner &= maskCorner;

            twist += cornerTwist[i];
            twist = twist % 3;

            corner |= (twist << 3);
        }

        if (n != 2) edge ^= maskEdgeTwist << 4;

        c[i] = corner;
        e[i] = edge;
    }

    for (int i = 0; i < 4; i++) {
        int j = (i - n + 4) % 4;

        int shiftCorner = cornersMoves[face][i] * 5;
        uint64_t maskCorner = maskSquare << shiftCorner;
        corners = (corners & ~maskCorner) | (c[j] << shiftCorner);

        int shiftEdge = edgesMoves[face][i] * 5;
        uint64_t maskEdge = maskSquare << shiftEdge;
        edges = (edges & ~maskEdge) | (e[j] << shiftEdge);
    }

    addMove(face, n);
    g++;
}

//U, D, F, L, B, R
const array<int[4], 6> Cube::cornersMoves = { {
    {0, 1, 3, 2},
    {6, 7, 5, 4},
    {2, 3, 7, 6},
    {0, 2, 6, 4},
    {1, 0, 4, 5},
    {3, 1, 5, 7},
} };

const array<int[4], 6> Cube::edgesMoves = { {
    {0, 1, 2, 3},
    {10, 9, 8, 11},
    {2, 6, 10, 7},
    {3, 7, 11, 4},
    {0, 4, 8, 5},
    {1, 5, 9, 6},
} };

const int Cube::cornerTwist[4] = { 1, 2, 1, 2 };

uint8_t Cube::getCorner(int i) {
    return (corners >> i * 5) & maskSquare;
}

uint8_t Cube::getEdge(int i) {
    return (edges >> i * 5) & maskSquare;
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
    uint64_t standardizedEdges = Symmetry::standardizeEdges(edges);
    return CubeKey{ standardizedCorners, standardizedEdges };
}

CornerKey Cube::toCornerKey() {
    uint64_t standardized = Symmetry::standardizeCorners(corners);
    return CornerKey{ standardized };
}

EdgeKey Cube::toEdgeKey(int n) {
    uint64_t standardized = Symmetry::standardizeEdges(edges);;
    n *= 6;

    for (int i = 0; i < 12; i++) {
        uint8_t edge = (standardized >> i * 5) & 0b01111;

        if (edge >= n && edge < n + 6) continue;

        standardized &= ~(maskSquare << (i * 5));
    }

    return EdgeKey{ standardized };
}
