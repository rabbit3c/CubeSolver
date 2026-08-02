#pragma once

#include <cmath>
#include "dense_database.h"
#include "../corners.h"

class CornerDatabase : public DenseDatabase {
public:
    CornerDatabase(int depth) : DenseDatabase(depth, 88179840) {
        name = "Corner";
    };

protected:
    int pow3[8] = { 1, 3, 9, 27, 81, 243, 729, 2187 };

    string getPath() {
        return "databases/corner/database";
    };

    int getKey(Cube& cube) {
        return rankPermutation(cube.corners);
    }

    int rankPermutation(uint64_t corners) {
        array<uint8_t, 8> ids = Corners::getCornerIDs(corners);

        int rank = 0;
        for (int i = 7; i >= 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                if (ids[j] > ids[i]) ids[j]--;
            }

            rank += ids[i] * factorial(i);
        }

        rank *= pow3[7];

        array<uint8_t, 8> orientations = Corners::getCornerOrientations(corners);
        for (int i = 6; i >= 0; i--) {
            rank += orientations[i] * pow3[i];
        }

        if (rank >= 88179840) throw invalid_argument("rank too high");;
        return rank;
    }
};