#include <thread>
#include <vector>
#include <mutex>
#include "cube.h"
#include "solver.h"
#include "scrambler.h"

Solver::Solver(int maxDepth, bool logging) : maxDepth(maxDepth), logging(logging) {}

void Solver::multisolve(Cube cube) {
    solved = false;
    int depth = maxDepth - 1;

    vector<thread> threads;
    for (int i = 0; i < 6; i++) {
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
    depth -= 1;
    if (depth < 0) return false;

    for (int i = 0; i < 6; i++) {
        if (i == lastMove) continue;
        if (i == 1 && lastMove == 0) continue;
        if (i == 5 && lastMove == 3) continue;
        if (i == 4 && lastMove == 2) continue;

        for (int n = 1; n <= 3; n++) {
            const bool result = tryMove(cube, i, n, depth);
            if (result) return true;
        }
    }

    return false;
}


bool Solver::tryMove(Cube cube, int i, int n, int depth) {
    Cube newCube = cube;
    newCube.move(i, n);

    //Check if the cube is in the database
    int result = patternDatabase.check(newCube);
    if (result != -1) {
        if (!solved) solved = true;

        if (result < cube.distance) cube.distance = result;
        else return false;
    }
    else if (solved) return false;
    else if (patternDatabase.depthDB >= depth) return false;

    if (newCube.completion()) {
        if (logging) {
            lock_guard<mutex> lock(printMutex);
            cout << "Cube solved" << endl;
            cout << "Solution: " << endl;
            newCube.printMoves();
        }

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
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Done!" << endl;

    cout << endl;
    cout << "Total time: " << duration.count() << "ms" << endl;
    cout << "Average time: " << duration.count() / runs << "ms" << endl;
}
