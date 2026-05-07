#include "cube.h"
#include <array>

Cube::Cube() {
    for (int i = 0; i < 9; i++) {
        cube[i] = static_cast<color>(i);
    }
}

void Cube::print() {
    for (Side side : cube) {
        side.print();
    }
}

bool Cube::completion() {
    for (Side side : cube) {
        if (!side.completion()) return false;
    }
    
    return true;
}