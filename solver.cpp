#include <thread>
#include <vector>
#include <mutex>
#include "cube.h"
#include "solver.h"
#include "scrambler.h"
#include <algorithm>

Solver::Solver(int maxDepth, bool logging) : maxDepth(maxDepth), logging(logging) {}

void Solver::multisolve(Cube cube) {
    solved = false;

    vector<thread> threads;
    for (int i = 0; i < 6; i++) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([this, cube, i, n]() {
                Cube newCube = cube;
                newCube.move(i, n);

                const bool completed = checkCompletion(newCube);

                const bool result = checkDatabase(newCube);
                if (result) solve(newCube, i);
                }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    if (solved) return;

    if (margin > 14) {
        cout << "No Solution was found" << endl;
        return;
    }

    if (logging) {
        cout << "No Solution found with margin: " << margin << endl;
        cout << "Trying with margin " << margin + 2 << endl << endl;
    }

    margin += 2;
    multisolve(cube);
}

bool Solver::solve(Cube cube, int lastMove) {
    if (cube.g >= maxDepth) return false;

    vector<Cube> cubes;

    for (int i = 0; i < 6; i++) {
        if (i == lastMove) continue;
        if (i == 1 && lastMove == 0) continue;
        if (i == 5 && lastMove == 3) continue;
        if (i == 4 && lastMove == 2) continue;

        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            newCube.move(i, n);

            const bool completed = checkCompletion(newCube);
            if (completed) return true;

            const bool result = checkDatabase(newCube);
            if (!result) continue;

            newCube.evaluate();

            if (newCube.h + newCube.g > maxDepth + margin) newCube.strikes++;
            else newCube.strikes = 0;

            if (!solved & (newCube.strikes >= maxStrikes)) continue;

            cubes.push_back(newCube);
        }
    }

    sort(cubes.begin(), cubes.end(), [](const Cube& a, const Cube& b) { return a.h < b.h; });

    for (Cube cube : cubes) {
        const bool result = solve(cube, cube.lastMove());
        if (result) return true;
    }

    return false;
}

bool Solver::checkCompletion(Cube& cube) {
    if (!cube.completion()) return false;

    if (!solved) solved = true;

    if (logging) {
        lock_guard<mutex> lock(printMutex);
        cout << "Cube solved" << endl;
        cout << "Solution: " << endl;
        cube.printMoves();
    }

    return true;
}

bool Solver::checkDatabase(Cube& cube) {
    int result = patternDatabase.check(cube);
    if (result != -1) {
        if (!solved) solved = true;

        //if (result < cube.distance) cube.distance = result;
        //else if (result > cube.distance) return false; //I don't know why it doesn't really work.
    }
    else if (solved) return false;
    else if (patternDatabase.depthDB >= maxDepth - cube.g) return false;

    return true;
}

void test(int depth, int runs) {
    Solver solver = Solver(depth, false);
    vector<Cube> cubes;

    cout << "Creating Cubes" << endl;
    for (int i = 0; i < runs; i++) {
        Cube cube = Cube();
        scramble(cube, depth, false);
        cubes.push_back(cube);
    }

    cout << "Starting..." << endl;
    auto start = chrono::system_clock::now();

    for (int i = 0; i < runs; i++) {
        solver.multisolve(cubes[i]);

        cout << "Run " << i + 1 << " done" << endl;
    }

    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Done!" << endl;

    cout << endl;
    cout << "Total time: " << duration.count() << "ms" << endl;
    cout << "Average time: " << duration.count() / runs << "ms" << endl;
}
