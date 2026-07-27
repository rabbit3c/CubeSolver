#include <thread>
#include "cube.h"
#include "solver.h"
#include "scrambler.h"
#include <algorithm>
#include <numeric>

Solver::Solver(int maxDepth, bool logging) : maxDepth(maxDepth), logging(logging) {
    edgeDatabaseA.init();
    edgeDatabaseB.init();
    endDatabase.init();
    cornerDatabase.init();
}

void Solver::multisolve(Cube cube) {
    auto start = chrono::system_clock::now();

    found = false;
    solved = false;
    nodes = 0;

    vector<thread> threads;
    for (int i = 0; i < 6; i++) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([this, cube, i, n]() {
                Cube newCube = cube;
                newCube.move(i, n);

                checkCompletion(newCube);

                int nodesThread = 1;
                solve(newCube, i, nodesThread);
                nodes += nodesThread;
                }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    if (solved) {
        cout << endl << "Amount of explored nodes: " << nodes << endl;
        cout << "Nodes per second: " << nodes / duration.count() * 1000 << "/s" << endl;
        return;
    }

    if (found) {
        cout << "Cube was found in the Database but not solved" << endl;
    }

    if (depth >= maxDepth) {
        cout << "No Solution was found" << endl;
        return;
    }

    if (logging) {
        cout << "No solution found at depth " << depth << endl;
        cout << "Trying with depth " << depth + 1 << endl << endl;
    }

    depth++;
    multisolve(cube);
}

bool Solver::solve(Cube cube, int lastMove, int& nodesThread) {
    if (cube.g >= depth) return false;
    nodesThread++;

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

            const bool result = evaluate(newCube);
            if (!result) continue;

            cubes.push_back(newCube);
        }
    }

    sort(cubes.begin(), cubes.end(), [](const Cube& a, const Cube& b) {
        if (a.h != b.h) return a.h < b.h;
        else return a.hAverage < b.hAverage;
        });

    for (Cube cube : cubes) {
        const bool result = solve(cube, cube.lastMove(), nodesThread);
        if (result) return true;
    }

    return false;
}

bool Solver::checkCompletion(Cube& cube) {
    if (!cube.completion()) return false;

    if (!found) found = true;
    if (!solved) solved = true;

    if (logging) {
        lock_guard<mutex> lock(printMutex);
        cout << "Cube solved" << endl << endl;
        cout << "Solution: " << endl;
        cube.printMoves();
    }

    return true;
}

bool Solver::evaluate(Cube& cube) {
    int endH = endDatabase.check(cube);

    if (endH != -1) {
        if (!found) found = true;

        cube.h = endH;
        return true;
    }
    else if (found) return false;
    else if (endDatabase.depthDB >= depth - cube.g) return false;

    auto arrayH = getArrayH(cube);
    cube.h = *max_element(arrayH.begin(), arrayH.end());
    cube.hAverage = accumulate(arrayH.begin(), arrayH.end(), 0);

    if (!found & (cube.h + cube.g > depth)) return false;
    return true;
}

array<int, 3> Solver::getArrayH(Cube& cube) {
    return { cornerDatabase.check(cube), edgeDatabaseA.check(cube), edgeDatabaseB.check(cube) };
}

void test(int depth, int runs) {
    Solver solver = Solver(depth, true);
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
        solver.depth = 12;
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
