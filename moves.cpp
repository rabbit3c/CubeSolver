#include "moves.h"

namespace Moves {
    void generate() {
        cout << "Generating Move Lookup Tables..." << endl;
        generateMasks();
        generateCornerIDsLookup();
        generateCornerTwistsLookup();
        generateEdgeIDsLookup();
        generateEdgeTwistsLookup();
        cout << "Generated Move Lookup Tables" << endl << endl;
    }

    void generateMasks() {
        for (int i = 0; i < 6; i++) {
            uint64_t mask = 0;
            uint64_t maskTwists = 0;

            for (int corner : cornersMoves[i]) {
                mask |= maskCornerID << corner * 5 + 2;
                maskTwists |= maskCornerTwist << corner * 5;
            }

            masksCornerIDs[i] = mask;
            masksCornerTwists[i] = maskTwists;
        }

        for (int i = 0; i < 6; i++) {
            uint64_t mask = 0;
            uint64_t maskTwists = 0;

            for (int edge : edgesMoves[i]) {
                mask |= maskEdgeID << edge * 5 + 1;
                maskTwists |= maskEdgeTwist << edge * 5;
            }

            masksEdgeIDs[i] = mask;
            masksEdgeTwists[i] = maskTwists;
        }
    }

    void generateLookup(const vector<uint8_t>& options, const bool unique, const array<array<int, 4>, 6>& moves, int offset,
        function<uint64_t(array<uint8_t, 4>, int, int, int)> func, array<array<unordered_map<uint64_t, uint64_t>, 3>, 6>& lookupTable) {
        for (int face = 0; face < 6; face++) {
            // generate all P(8,4) arrangements of 4 distinct corner IDs into the 4 affected slots
            for (uint8_t a : options) for (uint8_t b : options) for (uint8_t c : options) for (uint8_t d : options) {
                if (unique) {
                    if (a == b || a == c || a == d || b == c || b == d || c == d) continue;  // all pieces must be different
                }

                array<uint8_t, 4> ids = { a, b, c, d };
                uint64_t key = 0;

                for (int i = 0; i < 4; i++) key |= (uint64_t)ids[i] << (moves[face][i] * 5 + offset);

                for (int n = 1; n <= 3; n++) {
                    uint64_t result = 0;

                    for (int i = 0; i < 4; i++) {
                        int j = nextIndex(i, n);
                        result |= func(ids, face, n, i) << (moves[face][j] * 5 + offset);
                    }

                    if (lookupTable[face][n - 1].count(key) != 0) throw invalid_argument("Already in Lookup Table");
                    lookupTable[face][n - 1][key] = result;
                }
            }
        }

        array<uint64_t, 8> ids = { 0, 1, 2, 3, 4, 5, 6, 7 };
    }

    uint64_t funcID(array<uint8_t, 4> ids, int face, int n, int i) {
        return (uint64_t)ids[i];
    }

    uint64_t funcCornerTwist(array<uint8_t, 4> ids, int face, int n, int i) {
        uint64_t twist = ids[i];

        if (face > 1 && n != 2) {
            twist += twistCorners[i];
            twist = twist % 3;
        }

        return twist;
    }

    uint64_t funcEdgeTwist(array<uint8_t, 4> ids, int face, int n, int i) {
        uint64_t twist = ids[i];

        if (n != 2) twist ^= 0b1;

        return twist;
    }

    void generateCornerIDsLookup() {
        vector<uint8_t> options = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        generateLookup(options, true, cornersMoves, 2, funcID, cornerIDsLookup);
    }

    void generateCornerTwistsLookup() {
        vector<uint8_t> options = { 0, 1, 2 };
        generateLookup(options, false, cornersMoves, 0, funcCornerTwist, cornerTwistsLookup);
    }

    void generateEdgeIDsLookup() {
        vector<uint8_t> options = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        generateLookup(options, true, edgesMoves, 1, funcID, edgeIDsLookup);
    }

    void generateEdgeTwistsLookup() {
        vector<uint8_t> options = { 0, 1 };
        generateLookup(options, false, edgesMoves, 0, funcEdgeTwist, edgeTwistsLookup);
    }

    uint64_t getCornersFromArray(array<uint64_t, 8>& ids) {
        uint64_t corners = 0;

        for (int i = 0; i < 8; i++) {
            corners |= ids[i] << i * 5 + 2;
        }

        return corners;
    }

    int nextIndex(int i, int n) {
        return (i + n) % 4;
    }
}