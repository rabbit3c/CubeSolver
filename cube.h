#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>

using namespace std;

struct CubeKey {
    uint64_t corners;
    uint64_t edges;
    bool operator==(const CubeKey& other) const {
        return corners == other.corners && edges == other.edges;
    }
};

struct CubeKeyHash {
    size_t operator()(const CubeKey& cubeKey) const {
        size_t h1 = hash<uint64_t>{}(cubeKey.corners);
        size_t h2 = hash<uint64_t>{}(cubeKey.edges);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

struct CornerKey {
    uint64_t corners;
    bool operator==(const CornerKey& other) const {
        return corners == other.corners;
    }
};

struct CornerKeyHash {
    size_t operator()(const CornerKey& cornerKey) const {
        size_t h = hash<uint64_t>{}(cornerKey.corners);
        return h;
    }
};

class Cube {
public:
    //New representation of the cube
    uint64_t corners; //3bits for the corner (0 - 7) followed by 2 bits for the 3 orientations (0 - 2) -> 5 bits per corner -> 40 total
    //Numeration starts with the top left corner from the white side in clockwise (U direction). The corners on the yellow side are numerated parallel to the top corners
    //0w1 xxx 4y5
    //www xxx yyy
    //2w3 xxx 6y7

    uint64_t edges; //4bits for the edge (0 - 11) followed by 1 bit for the orientation (0 - 1) -> 5 bits per edge -> 60 total
    //Numeration starts with the top edge from the wide side in clockwise (U direction). the other edges are numerated parallel to the top edges.
    //w0w 4x5 y8y
    //3w1 xxx By9
    //w2w 7x6 yAy

    vector<uint8_t> moves;

    uint8_t distance = 0;

    int h = 1000;
    int g = 0;
    int strikes = 0;

    Cube();

    void move(int face, int n);

    void print();
    void printMoves();
    int lastMove();

    void clearMoves();

    bool completion();

    CubeKey toKey();
    CornerKey toCornerKey();

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

    void addMove(int c, int n);
};