#pragma once

#include <array>
#include <cstdint>

using namespace std;

namespace Corners {
    inline uint64_t cornerMask = 0b11111;

    array<uint8_t, 8> getCornerIDs(uint64_t corners);
    array<uint8_t, 8> getCornerOrientations(uint64_t corners);

    uint8_t getCorner(uint64_t corners, int i);

    uint8_t getCornerID(uint64_t corners, int i);

    uint8_t getCornerOrientation(uint64_t corners, int i);
}