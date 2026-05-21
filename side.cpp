#include "side.h"
#include <iostream>
#include <stdexcept>

using namespace std;

uint32_t Side::square_mask = (1 << 3) - 1;
uint32_t Side::line_mask = (1 << 9) - 1;
uint32_t Side::ring_mask = (1 << 24) - 1;
uint32_t Side::side_mask = (1 << 27) - 1;

Side::Side(color c) {
    side = c;
    for (int i = 1; i < 9; i++) side += (c << (i * 3));
}

//Clockwise Rotation
void Side::rotate(int n) {
    uint32_t center = side & 0b111;
    uint32_t ring = side >> 3;
    uint32_t shift = n * 6;
    ring = ((ring << shift) | (ring >> (24 - shift))) & ring_mask;
    side = center | (ring << 3);
}

uint32_t Side::getLine(int n) {
    side &= side_mask;
    uint32_t shift = n * 6 + 3;
    uint32_t line = (side >> shift) & line_mask;
    if (n == 3) line |= ((side >> 3) & square_mask) << 6;
    return line;
}

void Side::replaceLine(int n, int line) {
    uint32_t shift = n * 6 + 3;
    uint32_t mask = line_mask << shift; 
    side = (side & ~mask) | (line << shift);
    if (n == 3) side = (side & ~(square_mask << 3)) | (((line >> 6) & square_mask) << 3); 
}

void Side::print() {
    string stringSide = "";
    int order[9] = {1, 2, 3, 8, 0, 4, 7, 6, 5};

    for (int i = 0; i < 9; i++) {
        color c = static_cast<color>((side >> (order[i] * 3)) & square_mask);
        stringSide.append(colorToString(c));

        if ((i + 1) % 3 == 0) stringSide.append("\n");
    }

    cout << stringSide << endl;
}

bool Side::completion() {
    uint32_t c = side & square_mask;
    for (int i = 1; i < 9; i++) {
        if (((side >> (i * 3)) & square_mask) != c) return false;
    }

    return true;
}