#include <thread>
#include <vector>
#include <mutex>
#include "cube.h"
#include "solver.h"

mutex Solver::printMutex;

vector<void(Cube::*)(int)> Solver::options = {
    &Cube::up, &Cube::down, &Cube::right, &Cube::left, &Cube::front, &Cube::back
};

void Solver::multisolve(Cube cube, int maxDepth) {
    int depth = 1;

    vector<thread> threads;
    for (void(Cube::*option)(int) : options) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([cube, option, n, maxDepth, depth]() {
                tryMove(cube, option, n, maxDepth, depth);
            }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}


void Solver::solve(Cube cube, int maxDepth, int depth) {
    depth += 1;
    if (depth > maxDepth) return;

    for (void(Cube::*option)(int) : options) {
        for (int n = 1; n <= 3; n++) {
            tryMove(cube, option, n, maxDepth, depth);
        }
    }
}


void Solver::tryMove(Cube cube, void(Cube::*option)(int), int n, int maxDepth, int depth) {
    Cube newCube = cube;
    (newCube.*option)(n);

    if (newCube.completion()) {
        lock_guard<mutex> lock(printMutex);
        cout << "Cube solved" << endl;
        cout << "Solution: " << endl;
        newCube.printMoves();
        
        return;
    }
    
    solve(newCube, maxDepth, depth);
}