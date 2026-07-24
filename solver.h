#pragma once

#include <mutex>
#include <vector>
#include <atomic>
#include "cube.h"
#include "end_database.h"
#include "corner_database.h"
#include "edge_database.h"

class Solver {
public:
    Solver(int maxDepth, bool logging = true);

    bool solve(Cube cube, int lastMove);
    void multisolve(Cube cube);

private:
    EndDatabase endDatabase = EndDatabase(7);
    CornerDatabase cornerDatabase = CornerDatabase(20);
    EdgeDatabase edgeDatabase0 = EdgeDatabase(20, 0);
    EdgeDatabase edgeDatabase1 = EdgeDatabase(20, 1);

    atomic<bool> found{ false };
    atomic<bool> solved{ false };

    mutex printMutex;
    int maxDepth;

    bool logging;

    bool checkCompletion(Cube& cube);
    bool checkDatabase(Cube& cube);

    void evaluate(Cube& cube);
};

void test(int depth, int runs);