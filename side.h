#pragma once

#include <array>
#include <cstdint>
#include "colors.h"

class Side {
    public:
        Side() = default;

        Side(color c);

        void rotate(int n);

        uint32_t getLine(int n);

        void replaceLine(int n, int line);

        bool completion();

        void print();
        
    private:
        uint32_t side;
        //1 2 3   03 06 09
        //8 0 4   24 00 12
        //7 6 5   21 18 15

        static uint32_t square_mask;
        static uint32_t line_mask;
        static uint32_t ring_mask;
        static uint32_t side_mask;
};