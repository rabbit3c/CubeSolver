#include "symmetry.h"


namespace Symmetry {
    uint64_t applyCornerSymmetry(uint64_t corners, int i, uint64_t max) {
        array<uint8_t, 8>& symmetry = cornerSymmetries[i];

        uint64_t newCorners = 0;

        for (int i = 7; i >= 0; i--) {
            uint8_t oldCorner = (corners >> i * 5) & 0b11111;
            uint64_t newCorner = symmetry[(oldCorner >> 2) & 0b111] << 2;

            newCorner |= (oldCorner & 0b11);
            newCorners |= (newCorner << symmetry[i] * 5);

            if (newCorners > max) break;
        }

        return newCorners;
    }

    uint64_t applyEdgeSymmetry(uint64_t edges, int i, uint64_t max) {
        array<uint8_t, 12>& symmetry = edgeSymmetries[i];

        uint64_t newEdges = 0;

        for (int i = 11; i >= 0; i--) {
            uint8_t oldEdge = (edges >> i * 5) & 0b11111;
            uint64_t newEdge = symmetry[(oldEdge >> 1) & 0b1111] << 1;

            newEdge |= (oldEdge & 0b1);
            newEdges |= (newEdge << symmetry[i] * 5);

            if (newEdges > max) break;
        }

        return newEdges;
    }

    uint64_t standardizeCorners(uint64_t corners) {
        uint64_t standardized = corners;

        for (int i = 1; i < cornerSymmetries.size(); i++) {
            uint64_t newCorners = applyCornerSymmetry(corners, i, standardized);
            if (newCorners < standardized) standardized = newCorners;
        }

        return standardized;
    }

    uint64_t standardizeEdges(uint64_t edges, int maxSymmetry) {
        uint64_t standardized = edges;

        for (int i = 1; i < maxSymmetry; i++) {
            uint64_t newEdges = applyEdgeSymmetry(edges, i, standardized);
            if (newEdges < standardized) standardized = newEdges;
        }

        return standardized;
    }

    void printCorners(uint64_t corners) {
        for (int i = 0; i < 8; i++) {
            uint8_t corner = (corners >> (i * 5 + 2)) & 0b111;
            cout << to_string(corner) << " ";
        }

        cout << endl << endl;
    }
}

