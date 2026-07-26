#include "symmetry.h"


namespace Symmetry {
    void generateCornerSymmetries() {
        cornerSymmetries.clear();

        for (int n = 0; n < 7; n++) {
            array<uint8_t, 8> symmetry;

            for (int i = 0; i < 8; i++) {
                int x = equalBits(i) ? 0 : 1;
                symmetry[i] = i ^ xors[n * 2 + x];
            }

            cornerSymmetries.push_back(symmetry);
        }
    }

    bool equalBits(int i) {
        return (i & 1) == ((i >> 1) & 1);
    }

    uint64_t applyCornerSymmetry(uint64_t corners, int i) {
        array<uint8_t, 8>& symmetry = cornerSymmetries[i];

        uint64_t newCorners = 0;

        for (int i = 0; i < 8; i++) {
            uint8_t oldCorner = (corners >> i * 5) & 0b11111;
            uint64_t newCorner = symmetry[oldCorner & 0b111];

            newCorner |= (oldCorner & 0b11000);

            newCorners |= (newCorner << symmetry[i] * 5);
        }

        return newCorners;
    }

    uint64_t standardizeCorners(uint64_t corners) {
        uint64_t standardized = corners;

        for (int i = 0; i < cornerSymmetries.size(); i++) {
            uint64_t newCorners = applyCornerSymmetry(corners, i);
            if (newCorners < standardized) standardized = newCorners;
        }

        return standardized;
    }

    void printCorners(uint64_t corners) {
        for (int i = 0; i < 8; i++) {
            uint8_t corner = (corners >> i * 5) & 0b111;
            cout << to_string(corner) << " ";
        }

        cout << endl << endl;
    }
}

