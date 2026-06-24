#pragma once

#include "side.h"
#include <array>
#include <vector>

class Cube {
    public:
        array<Side, 6> cube;
        vector<uint8_t> moves;

        Cube();
        
        void move(int face, int n);

        void print();
        void printMoves();

        void clearMoves();

        bool completion();

        array<uint32_t, 6> toKey();

    private:
        static const array<int[4], 6> sides;
        static const array<int[4], 6> nLines;

        void addMove(int c, int n);
};