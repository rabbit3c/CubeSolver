#pragma 
#include <mutex>
#include <vector>
#include <atomic>
#include "cube.h"
#include "pattern_database.h"

class Solver {
    public:
        Solver(int maxDepth, bool logging=true);

        bool solve(Cube cube, int depth, int lastMove);
        void multisolve(Cube cube);
        
    private:
        PatternDatabase patternDatabase = PatternDatabase(6);

        atomic<bool> solved{false};

        mutex printMutex;
        int maxDepth;
        bool logging;

        bool tryMove(Cube cube, int i, int n, int depth);
};

void test(int depth, int runs);