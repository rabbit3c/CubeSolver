#pragma once

#include <cstdint>
#include <array>

using namespace std;

namespace Edges {
    array<uint8_t, 12> getEdgeIDs(uint64_t edges);
    array<uint8_t, 12> getEdgeOrientations(uint64_t edges);

    uint8_t getEdgeID(uint64_t edges, int i);
    uint8_t getEdgeOrientation(uint64_t edges, int i);
}