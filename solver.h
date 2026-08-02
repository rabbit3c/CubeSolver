#pragma once

#include <mutex>
#include <vector>
#include <atomic>
#include "cube.h"
#include "databases/end_database.h"
#include "databases/corner_database.h"
#include "databases/edge_database.h"

class Solver {
public:
    Solver(int maxDepth, bool logging = true);

    int depth;

    bool solve(Cube cube, int lastMove, int& nodes);
    void multisolve(Cube cube);

private:
    EndDatabase endDatabase = EndDatabase(8);
    CornerDatabase cornerDatabase = CornerDatabase(12);
    EdgeDatabase edgeDatabaseA = EdgeDatabase(12, 0);

    atomic<bool> found{ false };
    atomic<bool> solved{ false };

    atomic<int> nodes{ false };

    mutex printMutex;
    int maxDepth;

    bool logging;

    bool checkCompletion(Cube& cube);

    bool evaluate(Cube& cube);
    array<int, 3> getArrayH(Cube& cube);
};

void test(int depth, int runs);