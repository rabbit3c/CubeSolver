#pragma once

#include "side.h"
#include <array>
#include <vector>

class Cube {
public:
    array<Side, 6> cube;
    vector<uint8_t> moves;

    uint8_t distance = 0;

    float h = 1000;
    int g = 0;

    Cube();

    void move(int face, int n);

    void print();
    void printMoves();
    int lastMove();

    void clearMoves();

    bool completion();

    float evaluate();

    array<uint32_t, 6> toKey();
    size_t toHash();

private:
    static const array<int[4], 6> sides;
    static const array<int[4], 6> nLines;

    void addMove(int c, int n);
};