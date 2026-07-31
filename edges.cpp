#include "edges.h"

namespace Edges {
    array<uint8_t, 12> getEdgeIDs(uint64_t edges) {
        array<uint8_t, 12> edgeIDs;
        for (int i = 0; i < 12; i++) {
            edgeIDs[i] = getEdgeID(edges, i);
        }
        return edgeIDs;
    }

    array<uint8_t, 12> getEdgeOrientations(uint64_t edges) {
        array<uint8_t, 12> edgeOrientations;
        for (int i = 0; i < 12; i++) {
            edgeOrientations[i] = getEdgeOrientation(edges, i);
        }
        return edgeOrientations;
    }

    uint8_t getEdgeID(uint64_t edges, int i) {
        return (edges >> (i * 5 + 1)) & 0b1111;
    }

    uint8_t getEdgeOrientation(uint64_t edges, int i) {
        return (edges >> i * 5) & 0b1;
    }
}