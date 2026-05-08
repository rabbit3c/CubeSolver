#pragma once

#include <array>
#include "colors.h"

class Side {
    public:
        Side() = default;

        Side(color c);

        Side(color colors[9]);

        bool reverseCol = false;
        bool reverseLine = false;

        void rotate(int n);

        array<color, 3> getLine(int n);

        array<color, 3> getCol(int n);

        void replaceLine(int n, array<color, 3> line);

        void replaceCol(int n, array<color, 3> col);

        bool completion();

        void print();
        
    private:
        array<color, 9> side;

        static const int rotations[3][9];

        static array<color, 3> reverse(array<color, 3> line);
};