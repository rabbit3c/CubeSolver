#pragma once
#include "cube.h"
#include <mutex>
#include <vector>

class Solver {
    public:
        static void solve(Cube cube, int maxDepth, int depth);
        static void multisolve(Cube cube, int maxDepth);
    private:
        static mutex printMutex;
        static vector<void(Cube::*)(int)> options;

        static void tryMove(Cube cube, void(Cube::*option)(int), int n, int maxDepth, int depth);
};