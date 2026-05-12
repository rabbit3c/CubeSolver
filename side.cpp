#include "side.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Side::Side(color c) {
    side = c;
    for (int i = 1; i < 9; i++) side += (c << (i * 3));
}

//Clockwise Rotation
void Side::rotate(int n) {
    uint32_t center = side & 0b111;
    uint32_t ring = side >> 3;
    ring = ((ring << (n * 6)) | (ring >> (24 - n * 6))) & 0b111111111111111111111111;
    side = center | (ring << 3);
}

uint32_t Side::getLine(int n) {
    side &= 0b111111111111111111111111111;
    uint32_t line = (side >> (n * 6 + 3)) & 0b111111111;
    if (n == 3) line |= ((side >> 3) & 0b111) << 6;
    return line;
}

void Side::replaceLine(int n, int line) {
    uint32_t mask = 0b111111111u << (n * 6 + 3); 
    side = (side & ~mask) | (line << (n * 6 + 3));
    if (n == 3) side = (side & ~(0b111u << 3)) | (((line >> 6) & 0b111) << 3); 
}

void Side::print() {
    string stringSide = "";
    int order[9] = {1, 2, 3, 8, 0, 4, 7, 6, 5};

    for (int i = 0; i < 9; i++) {
        color c = static_cast<color>((side >> (order[i] * 3)) & 0b111);
        stringSide.append(colorToString(c));

        if ((i + 1) % 3 == 0) stringSide.append("\n");
    }

    cout << stringSide << endl;
}

bool Side::completion() {
    uint32_t c = side & 0b111;
    for (int i = 1; i < 9; i++) {
        if (((side >> (i * 3)) & 0b111) != c) return false;
    }

    return true;
}