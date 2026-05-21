#pragma 
#include <mutex>
#include <vector>
#include <atomic>
#include "cube.h"


class Solver {
    public:
        Solver(int maxDepth, bool logging=true);

        bool solve(Cube cube, int depth, int lastMove);
        void multisolve(Cube cube);
        
    private:
        static vector<void(Cube::*)(int)> options;

        atomic<bool> solved{false};

        mutex printMutex;
        int maxDepth;
        bool logging;

        bool tryMove(Cube cube, int i, int n, int depth);
};

void test(int depth, int runs);