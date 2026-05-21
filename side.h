#pragma once

#include <array>
#include <cstdint>
#include "colors.h"

class Side {
    public:
        Side() = default;

        Side(color c);

        void rotate(int n);

        uint16_t getLine(int n);

        void replaceLine(int n, int line);

        bool completion();

        void print();
        
    private:
        uint32_t center;
        uint32_t ring;
        //0 1 2   00 03 06
        //7 - 3   21 -- 09
        //6 5 4   18 15 12

        uint32_t ring_completed;

        static uint8_t square_mask;
        static uint16_t line_mask;
        static uint32_t ring_mask;
};