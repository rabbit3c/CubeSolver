#pragma once

#include "side.h"
#include <array>

class Cube {
    public:
        Cube();
        
        void up(int n = 1);
        void down(int n = 1);
        void right(int n = 1);
        void left(int n = 1);
        void front(int n = 1);
        void back(int n = 1);

        void print();

        bool completion();

    private:
        array<Side, 6> cube;
        void move(int face, int sides[4], bool types[4], int nLines[4], int n);
};