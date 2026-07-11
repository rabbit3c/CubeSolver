#include "side.h"
#include <iostream>
#include <stdexcept>

using namespace std;

uint8_t Side::square_mask = 0b111;
uint16_t Side::line_mask = (1 << 9) - 1;
uint32_t Side::ring_mask = (1 << 24) - 1;

Side::Side(color c) {
    center = c;
    ring_completed = c;
    for (int i = 1; i < 8; i++) ring_completed += (c << (i * 3));

    ring = ring_completed;
}

//Clockwise Rotation
void Side::rotate(int n) {
    int shift = n * 6;
    ring = ((ring << shift) | (ring >> (24 - shift))) & ring_mask;
}

uint16_t Side::getLine(int n) {
    const int shift = n * 6;
    uint32_t line = (ring >> shift) & line_mask;
    if (n == 3) line |= (ring & square_mask) << 6;
    return line;
}

void Side::replaceLine(int n, int line) {
    const int shift = n * 6;
    const uint32_t mask = line_mask << shift;
    ring = (ring & ~mask) | (line << shift);
    if (n == 3) ring = (ring & ~square_mask) | ((line >> 6) & square_mask);
    ring &= ring_mask;
}

void Side::print() {
    string stringSide = "";
    const int order[9] = { 0, 1, 2, 7, 0, 3, 6, 5, 4 };

    for (int i = 0; i < 9; i++) {
        if (i == 4) {
            color c = static_cast<color>(center);
            stringSide.append(colorToString(c));
            continue;
        }

        color c = static_cast<color>((ring >> (order[i] * 3)) & square_mask);
        stringSide.append(colorToString(c));

        if ((i + 1) % 3 == 0) stringSide.append("\n");
    }

    cout << stringSide << endl;
}

bool Side::completion() {
    return ring == ring_completed;
}

int Side::h() {
    int d = 8;
    for (int i = 0; i < 8; i++) {
        uint8_t square = (ring >> (i * 3)) & square_mask;
        if (square == center) d--;
    }
    return d;
}