#include "corners.h"

namespace Corners {
    array<uint8_t, 8> getCornerIDs(uint64_t corners) {
        array<uint8_t, 8> cornerIDs;
        for (int i = 0; i < 8; i++) {
            cornerIDs[i] = getCornerID(corners, i);
        }
        return cornerIDs;
    }

    array<uint8_t, 8> getCornerOrientations(uint64_t corners) {
        array<uint8_t, 8> cornerOrientations;
        for (int i = 0; i < 8; i++) {
            cornerOrientations[i] = getCornerOrientation(corners, i);
        }
        return cornerOrientations;
    }

    uint8_t getCorner(uint64_t corners, int i) {
        return (corners >> i * 5) & cornerMask;
    }

    uint8_t getCornerID(uint64_t corners, int i) {
        return (corners >> (i * 5 + 2)) & 0b111;
    }

    uint8_t getCornerOrientation(uint64_t corners, int i) {
        return (corners >> i * 5) & 0b11;
    }
}   