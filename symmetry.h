#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <iostream>

using namespace std;

namespace Symmetry {
    const array<uint8_t, 14> xorsCorner = { 0b011, 0b011, 0b010, 0b001, 0b001, 0b010, 0b101, 0b101, 0b110, 0b110, 0b111, 0b100, 0b100, 0b111 };
    const array<uint8_t, 6> xorsEdge = { 0b010, 0b010, 0b001, 0b011, 0b011, 0b001 };
    inline vector<array<uint8_t, 8>> cornerSymmetries;
    inline vector<array<uint8_t, 12>> edgeSymmetries;

    void generate();
    void generateCornerSymmetries();
    void generateEdgeSymmetries();

    uint64_t applyCornerSymmetry(uint64_t corners, int i);
    uint64_t applyEdgeSymmetry(uint64_t edges, int i);

    uint64_t standardizeCorners(uint64_t);
    uint64_t standardizeEdges(uint64_t);

    bool equalBits(int i);
};
