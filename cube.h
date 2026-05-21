#pragma once

#include "side.h"
#include <array>
#include <vector>

class Cube {
    public:
        array<Side, 6> cube;
        vector<uint8_t> moves;

        Cube();
        
        void up(int n = 1);
        void down(int n = 1);
        void right(int n = 1);
        void left(int n = 1);
        void front(int n = 1);
        void back(int n = 1);

        void print();
        void printMoves();

        void clearMoves();

        bool completion();

        array<uint32_t, 6> toKey();

    private:
        static array<int[4], 6> sides;
        static array<int[4], 6> nLines;

        void move(int face, int sides[4], int nLines[4], int n);
        void addMove(int c, int n);
};