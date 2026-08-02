#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>
#include <unordered_map>
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

private:
    static const uint64_t solvedCorners = 988530745472;
    static const uint64_t solvedEdges = 815802937703075904;

    void addMove(int c, int n);
};