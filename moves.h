#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>
#include <algorithm>

using namespace std;

namespace Moves {
    inline array<unordered_map<uint64_t, uint64_t>, 3> cornerIDsLookup;
    inline array<unordered_map<uint64_t, uint64_t>, 3> edgeIDsLookup;

    inline array<array<unordered_map<uint64_t, uint64_t>, 3>, 6> cornerTwistsLookup;
    inline array<array<unordered_map<uint64_t, uint64_t>, 3>, 6> edgeTwistsLookup;

    inline array<uint64_t, 6> masksCornerIDs;
    inline array<uint64_t, 6> masksEdgeIDs;

    inline array<uint64_t, 6> masksEdgeTwists;
    inline array<uint64_t, 6> masksCornerTwists;

    const uint64_t maskCornerID = 0b111;
    const uint64_t maskCornerTwist = 0b11;

    const uint64_t maskEdgeID = 0b1111;
    const uint64_t maskEdgeTwist = 0b1;

    const array<array<int, 4>, 6> cornersMoves = { {
        {0, 1, 3, 2},
        {6, 7, 5, 4},
        {2, 3, 7, 6},
        {0, 2, 6, 4},
        {1, 0, 4, 5},
        {3, 1, 5, 7},
    } };

    const array<array<int, 4>, 6> edgesMoves = { {
        {0, 1, 2, 3},
        {10, 9, 8, 11},
        {2, 6, 10, 7},
        {3, 7, 11, 4},
        {0, 4, 8, 5},
        {1, 5, 9, 6},
    } };

    const int twistCorners[4] = { 1, 2, 1, 2 };

    void generateMasks();
    void generateCornerIDsLookup();
    void generateCornerTwistsLookup();
    void generateEdgeIDsLookup();
    void generateEdgeTwistsLookup();

    void generate();

    uint64_t getCornersFromArray(array<uint64_t, 8>& ids);

    int nextIndex(int i, int n);
}