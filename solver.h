#pragma once
#include "cube.h"
#include <mutex>
#include <vector>

class Solver {
    public:
        Solver(int maxDepth, bool logging=true);

        void solve(Cube cube, int depth, int lastMove);
        void multisolve(Cube cube);
        
    private:
        static vector<void(Cube::*)(int)> options;

        mutex printMutex;
        int maxDepth;
        bool logging;

        void tryMove(Cube cube, int i, int n, int depth);
};

void test(int depth, int runs);