#include "moves.h"
#include "corners.h"
#include "edges.h"
#include <iostream>
#include <stdexcept>

namespace Moves {
    void generate() {
        cout << "Generating Move Lookup Tables..." << endl;

        generateMasks();
        cout << "Generated Masks" << endl;

        generateCornerIDsLookup();
        generateCornerTwistsLookup();
        cout << "Generated Corner Lookup" << endl;

        generateEdgeIDsLookup();
        generateEdgeTwistsLookup();
        cout << "Generated Edge Lookup" << endl;
        cout << "Done!" << endl << endl;
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

    void generateCornerIDsLookup() {
        array<uint8_t, 8> allIds = { 0, 1, 2, 3, 4, 5, 6, 7 };

        for (int face = 0; face < 6; face++) {
            // generate all P(8,4) arrangements of 4 distinct corner IDs into the 4 affected slots
            for (uint8_t a : allIds) for (uint8_t b : allIds) for (uint8_t c : allIds) for (uint8_t d : allIds) {
                if (a == b || a == c || a == d || b == c || b == d || c == d) continue;  // all pieces must be different

                array<uint8_t, 4> ids = { a, b, c, d };
                uint64_t key = 0;

                for (int i = 0; i < 4; i++) key |= (uint64_t)ids[i] << (cornersMoves[face][i] * 5 + 2);

                for (int n = 1; n <= 3; n++) {
                    uint64_t result = 0;

                    for (int i = 0; i < 4; i++) {
                        int j = nextIndex(i, n);
                        result |= ((uint64_t)ids[i] << (cornersMoves[face][j] * 5 + 2));
                    }

                    if (cornerIDsLookup[n - 1].count(key) != 0) throw invalid_argument("cornerIDsLookup");
                    cornerIDsLookup[n - 1][key] = result;
                }
            }
        }

        array<uint64_t, 8> ids = { 0, 1, 2, 3, 4, 5, 6, 7 };
    }

    void generateCornerTwistsLookup() {
        array<uint8_t, 3> allTwists = { 0, 1, 2 };

        for (int face = 0; face < 6; face++) {
            // generate all 3^4 arrangements of 4 distinct corner twists into the 4 affected slots
            for (uint8_t a : allTwists) for (uint8_t b : allTwists) for (uint8_t c : allTwists) for (uint8_t d : allTwists) {
                array<uint8_t, 4> twists = { a, b, c, d };
                uint64_t key = 0;

                for (int i = 0; i < 4; i++) key |= (uint64_t)twists[i] << (cornersMoves[face][i] * 5);

                for (int n = 1; n <= 3; n++) {
                    uint64_t result = 0;

                    for (int i = 0; i < 4; i++) {
                        uint64_t twist = twists[i];

                        if (face > 1 && n != 2) {
                            twist += twistCorners[i];
                            twist = twist % 3;
                        }

                        int j = nextIndex(i, n);
                        result |= (twist << (cornersMoves[face][j] * 5));
                    }

                    if (cornerTwistsLookup[face][n - 1].count(key) != 0) throw invalid_argument("cornerTwistsLookup");
                    cornerTwistsLookup[face][n - 1][key] = result;
                }
            }
        }
    }

    void generateEdgeIDsLookup() {
        array<uint8_t, 12> allIds = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

        for (int face = 0; face < 6; face++) {
            // generate all P(12,4) arrangements of 4 distinct edge IDs into the 4 affected slots
            for (uint8_t a : allIds) for (uint8_t b : allIds) for (uint8_t c : allIds) for (uint8_t d : allIds) {
                if (a == b || a == c || a == d || b == c || b == d || c == d) continue;  // all pieces must be different

                array<uint8_t, 4> ids = { a, b, c, d };
                uint64_t key = 0;

                for (int i = 0; i < 4; i++) key |= (uint64_t)ids[i] << (edgesMoves[face][i] * 5 + 1);

                for (int n = 1; n <= 3; n++) {
                    uint64_t result = 0;

                    for (int i = 0; i < 4; i++) {
                        int j = nextIndex(i, n);
                        result |= ((uint64_t)ids[i] << (edgesMoves[face][j] * 5 + 1));
                    }

                    if (edgeIDsLookup[n - 1].count(key) != 0) throw invalid_argument("Already Value for this key - edgeIDsLookup");
                    edgeIDsLookup[n - 1][key] = result;
                }
            }
        }
    }

    void generateEdgeTwistsLookup() {
        array<uint8_t, 2> allTwists = { 0, 1 };

        for (int face = 0; face < 6; face++) {
            // generate all 2^4 arrangements of 4 distinct corner twists into the 4 affected slots
            for (uint8_t a : allTwists) for (uint8_t b : allTwists) for (uint8_t c : allTwists) for (uint8_t d : allTwists) {
                array<uint8_t, 4> twists = { a, b, c, d };
                uint64_t key = 0;

                for (int i = 0; i < 4; i++) key |= (uint64_t)twists[i] << (edgesMoves[face][i] * 5);

                for (int n = 1; n <= 3; n++) {
                    uint64_t result = 0;

                    for (int i = 0; i < 4; i++) {
                        uint64_t twist = twists[i];

                        if (n != 2) twist ^= 0b1;

                        int j = nextIndex(i, n);
                        result |= (twist << (edgesMoves[face][j] * 5));
                    }

                    if (edgeTwistsLookup[face][n - 1].count(key) != 0) throw invalid_argument("edgeTwistsLookup");
                    edgeTwistsLookup[face][n - 1][key] = result;
                }
            }
        }
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