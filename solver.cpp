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
    solved = false;
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


bool Solver::solve(Cube cube, int depth, int lastMove) {
    depth += 1;
    if (depth > maxDepth) return false;

    for (int i = 0; i < options.size(); i++) {
        if (i == lastMove) continue;
        if (i + 1 == lastMove && i % 2 == 0) continue;

        for (int n = 1; n <= 3; n++) {
            bool solved = tryMove(cube, i, n, depth);
            if (solved) return true;
        }
    }

    return false;
}


bool Solver::tryMove(Cube cube, int i, int n, int depth) {
    if (solved) return false;

    Cube newCube = cube;
    (newCube.*options[i])(n);

    //Check if the cube is in the database, if it is, solves it automatically
    bool result = patternDatabase.check(newCube);
    
    if (!result && (patternDatabase.depthDB >= maxDepth - depth)) return false;

    if (newCube.completion()) {
        if (logging) {
            lock_guard<mutex> lock(printMutex);
            cout << "Cube solved" << endl;
            cout << "Solution: " << endl;
            newCube.printMoves();
        }
        
        solved = true;
        return true;
    }
    
    return solve(newCube, depth, i);
}

void test(int depth, int runs) {
    Solver solver = Solver(depth, false);
    vector<Cube> cubes;

    cout << "Creating Cubes" << endl;
    for (int i = 1; i <= runs; i++) {
        Cube cube = Cube();
        scramble(cube, depth, false);
        cubes.push_back(cube);
    }

    cout << "Starting..." << endl;
    auto start = chrono::system_clock::now();

    for (int i = 1; i <= runs; i++) {
        solver.multisolve(cubes[i]);

        cout << "Run " << i << " done" << endl;
    }

    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done!" << endl;

    cout << endl;
    cout << "Total time: " << duration.count() << "ms" << endl;
    cout << "Average time: " << duration.count() / runs << "ms" << endl;
}
