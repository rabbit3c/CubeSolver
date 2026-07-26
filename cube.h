#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>
#include "databases/database_keys.h"

using namespace std;

class Cube {
public:
    //New representation of the cube
    uint64_t corners; //3bits for the corner (0 - 7) followed by 2 bits for the 3 orientations (0 - 2) -> 5 bits per corner -> 40 total
    //Bit 0: L 0, R 0, Bit 1: B 0, F 1, Bit 2: U 0, D 1
    //0w1 xxx 4y5
    //www xxx yyy
    //2w3 xxx 6y7

    uint64_t edges; //4bits for the edge (0 - 11) followed by 1 bit for the orientation (0 - 1) -> 5 bits per edge -> 60 total
    //Numeration starts with the top edge from the wide side in clockwise (U direction). the other edges are numerated parallel to the top edges.
    //w0w 4x5 y8y
    //3w1 xxx By9
    //w2w 7x6 yAy

    vector<uint8_t> moves;

    int h;
    int hAverage;
    int g = 0;

    Cube();

    void move(int face, int n);

    void print();
    void printMoves();
    int lastMove();

    void clearMoves();

    bool completion();

    CubeKey toKey();
    CornerKey toCornerKey();
    EdgeKey toEdgeKey(int i);

    uint8_t getCorner(int i);

private:
    static const uint64_t solvedCorners = 247132686368;
    static const uint64_t solvedEdges = 407901468851537952;

    static const array<int[4], 6> cornersMoves; //U, D, F, L, B, R
    static const array<int[4], 6> edgesMoves;

    static const int cornerTwist[4];

    static const uint64_t maskSquare = 0b11111;
    static const uint64_t maskEdge = 0b1111;
    static const uint64_t maskCorner = 0b111;
    static const uint64_t maskCornerTwist = 0b11;
    static const uint64_t maskEdgeTwist = 0b1;

    uint8_t getEdge(int i);

    void addMove(int c, int n);
};