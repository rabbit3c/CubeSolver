#pragma once
#include "cube.h"
#include <mutex>
#include <vector>

class Solver {
    public:
        Solver(int maxDepth, bool logging=true);

        void solve(Cube cube, int depth);
        void multisolve(Cube cube);
        
    private:
        static vector<void(Cube::*)(int)> options;

        mutex printMutex;
        int maxDepth;
        bool logging;

        void tryMove(Cube cube, void(Cube::*option)(int), int n, int depth);
};

void test(int depth, int runs);