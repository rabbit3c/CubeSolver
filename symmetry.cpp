#include "symmetry.h"


namespace Symmetry {
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

    uint64_t applyEdgeSymmetry(uint64_t edges, int i) {
        array<uint8_t, 12>& symmetry = edgeSymmetries[i];

        uint64_t newEdges = 0;

        for (int i = 0; i < 12; i++) {
            uint8_t oldEdge = (edges >> i * 5) & 0b11111;
            uint64_t newEdge = symmetry[oldEdge & 0b1111];

            newEdge |= (oldEdge & 0b10000);

            newEdges |= (newEdge << symmetry[i] * 5);
        }

        return newEdges;
    }

    uint64_t standardizeCorners(uint64_t corners) {
        uint64_t standardized = corners;

        for (int i = 0; i < cornerSymmetries.size(); i++) {
            uint64_t newCorners = applyCornerSymmetry(corners, i);
            if (newCorners < standardized) standardized = newCorners;
        }

        return standardized;
    }

    uint64_t standardizeEdges(uint64_t edges) {
        uint64_t standardized = UINT64_MAX;

        for (int i = 0; i < edgeSymmetries.size(); i++) {
            uint64_t newEdges = applyEdgeSymmetry(edges, i);
            if (newEdges < standardized) standardized = newEdges;
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

