#include <thread>
#include <vector>
#include <mutex>
#include "cube.h"
#include "solver.h"
#include "scrambler.h"

vector<void(Cube::*)(int)> Solver::options = {
    &Cube::up, &Cube::down, &Cube::right, &Cube::left, &Cube::front, &Cube::back
};

Solver::Solver(int maxDepth, bool logging) : maxDepth(maxDepth), logging(logging) {}

void Solver::multisolve(Cube cube) {
    int depth = 1;

    vector<thread> threads;
    for (int i = 0; i < options.size(); i++) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([this, cube, i, n, depth]() {
                tryMove(cube, i, n, depth);
            }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}


void Solver::solve(Cube cube, int depth, int lastMove) {
    depth += 1;
    if (depth > maxDepth) return;

    for (int i = 0; i < options.size(); i++) {
        if (i == lastMove) continue;

        for (int n = 1; n <= 3; n++) {
            tryMove(cube, i, n, depth);
        }
    }
}


void Solver::tryMove(Cube cube, int i, int n, int depth) {
    Cube newCube = cube;
    (newCube.*options[i])(n);

    if (newCube.completion()) {
        if (logging) {
            lock_guard<mutex> lock(printMutex);
            cout << "Cube solved" << endl;
            cout << "Solution: " << endl;
            newCube.printMoves();
        }
        
        return;
    }
    
    solve(newCube, depth, i);
}

void test(int depth, int runs) {
    auto start = chrono::high_resolution_clock::now();

    Solver solver = Solver(depth, false);

    for (int n = 1; n <= runs; n++) {
        Cube cube = Cube();
        scramble(cube, depth, false);
        solver.multisolve(cube);

        cout << "Run " << n << " done" << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    cout << endl;
    cout << "Total time: " << duration.count() << "ms" << endl;
    cout << "Average time: " << duration.count() / runs << "ms" << endl;
}
