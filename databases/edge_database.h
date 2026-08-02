#pragma once

#include "dense_database.h"
#include "../edges.h"

class EdgeDatabase : public DenseDatabase {
public:
    EdgeDatabase(int depth, int i) : DenseDatabase(depth, 510935040), i(i) {
        name = "Edge " + to_string(i);
    };

protected:
    int i;

    string getPath() {
        return "databases/edge/database_" + to_string(i);
    };

    int getKey(Cube& cube) {
        return rankPermutation(cube.edges);
    }

    int rankPermutation(uint64_t edges) {
        array<uint8_t, 12> allIDs = Edges::getEdgeIDs(edges);
        array<uint8_t, 12> allOrientations = Edges::getEdgeOrientations(edges);

        array<uint8_t, 7> slots;
        array<uint8_t, 7> orientations;

        for (int i = 0; i < 12; i++) {
            uint8_t id = allIDs[i];

            if (id >= 7) continue;
            slots[id] = i;
            orientations[id] = allOrientations[i];
        }

        array<uint8_t, 12> remaining = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        int n = 12;
        int rank = 0;

        for (int i = 0; i < 7; i++) {
            int idx = -1;
            for (int k = 0; k < n; k++) {
                if (remaining[k] == slots[i]) { idx = k; break; }
            }

            rank = rank * n + idx;
            n--;

            for (int k = idx; k < n; k++) remaining[k] = remaining[k + 1];
        }

        rank = rank << 7;

        for (int i = 6; i >= 0; i--) {
            rank += orientations[i] << i;
        }

        if (rank >= 510935040) throw invalid_argument("rank too high");
        return rank;
    }
};