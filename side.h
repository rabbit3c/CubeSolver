#pragma once

#include <array>
#include "colors.h"

class Side {
    public:
        Side() = default;

        Side(color c);

        Side(color colors[9]);

        void print();
        
    private:
        array<array<color, 3>, 3> side;
};