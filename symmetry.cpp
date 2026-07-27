#include "symmetry.h"


namespace Symmetry {
    void generate() {
        generateCornerSymmetries();
        generateEdgeSymmetries();
    }

    void generateCornerSymmetries() {
        cornerSymmetries.clear();

        for (int n = 0; n < 7; n++) {
            array<uint8_t, 8> symmetry;

            for (int i = 0; i < 8; i++) {
                int x = equalBits(i) ? 0 : 1;
                symmetry[i] = i ^ xorsCorner[n * 2 + x];
            }

            cornerSymmetries.push_back(symmetry);
        }
    }

    void generateEdgeSymmetries() {
        edgeSymmetries.clear();

        for (int n = 0; n < 3; n++) {
            array<uint8_t, 12> symmetry;

            for (int i = 0; i < 12; i++) {
                int x = i % 2 ? 0 : 1;
                symmetry[i] = i ^ xorsEdge[n * 2 + x];
            }

            edgeSymmetries.push_back(symmetry);
        }

        array<uint8_t, 12> symmetry;

        for (int i = 0; i < 12; i++) {
            if (i > 3 && i < 8) {
                symmetry[i] = i ^ 0b0001;
            }
            else if (i % 2 == 0) {
                symmetry[i] = i ^ 0b1000;
            }
            else {
                symmetry[i] = i ^ 0b1010;
            }
        }

        edgeSymmetries.push_back(symmetry);

        array<uint8_t, 12> symmetry2;

        for (int i = 0; i < 12; i++) {
            if (i > 3 && i < 8) {
                symmetry2[i] = i ^ 0b0011;
            }
            else if (i % 2 == 0) {
                symmetry2[i] = i ^ 0b1010;
            }
            else {
                symmetry2[i] = i ^ 0b1000;
            }
        }

        edgeSymmetries.push_back(symmetry2);

        array<uint8_t, 12> symmetry3;

        for (int i = 0; i < 12; i++) {
            if (i > 3 && i < 8) {
                if (i % 2 == 0) {
                    symmetry3[i] = i ^ 0b0000;
                }
                else {
                    symmetry3[i] = i ^ 0b0010;
                }
            }
            else {
                symmetry3[i] = i ^ 0b1011;
            }
        }

        edgeSymmetries.push_back(symmetry3);

        array<uint8_t, 12> symmetry4;

        for (int i = 0; i < 12; i++) {
            if (i > 3 && i < 8) {
                if (i % 2 == 0) {
                    symmetry4[i] = i ^ 0b0010;
                }
                else {
                    symmetry4[i] = i ^ 0b0000;
                }
            }
            else {
                symmetry4[i] = i ^ 0b1001;
            }
        }

        edgeSymmetries.push_back(symmetry4);
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
        uint64_t standardized = edges;

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

