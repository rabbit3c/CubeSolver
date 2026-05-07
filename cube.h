#pragma once

#include "side.h"
#include <array>

class Cube {
    public:
        Cube();

        void print();

    private:
        array<Side, 6> cube;
};