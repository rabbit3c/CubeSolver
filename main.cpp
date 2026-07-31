#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"
#include "symmetry.h"

using namespace std;

void generateDatabases() {
    {
        auto edgeDatabase = EdgeDatabase(12, 0);
        edgeDatabase.init();
    }
    {
        auto endDatabase = EndDatabase(8);
        endDatabase.init();
    }
    {
        auto cornerDatabase = CornerDatabase(12);
        cornerDatabase.init();
    }
}

void testSymmetry() {
    int n = 0;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (i == j) continue;
            if (i < 2 && ((i ^ 0b001) == j)) continue;
            if (i > 1 && ((i ^ 0b110) == j)) continue;

            Cube cube = Cube();
            cube.move(j, 2);
            cube.move(i, 1);
            cube.move(j, 3);
            cube.move(i, 3);
            n++;

            cout << "Move " << n << ": " << endl;

            auto corners = Symmetry::standardizeCorners(cube.corners);
            cout << cube.corners << " -> ";
            cout << corners << endl;

            auto edges = Symmetry::standardizeEdges(cube.edges);
            cout << cube.edges << " -> ";
            cout << edges << endl << endl;
        }
    }
}

void testDatabases() {
    auto cornerDatabase = CornerDatabase(12);
    cornerDatabase.init();
    auto edgeDatabaseA = EdgeDatabase(12, 0);
    edgeDatabaseA.init();
    auto endDatabase = EndDatabase(8);
    endDatabase.init();

    for (int i = 0; i < 5; i++) {
        Cube cube = Cube();

        for (int i = 0; i < 20; i++) {
            scramble(cube, 1, false);

            cout << "Move " << i + 1 << ": ";
            cout << cornerDatabase.check(cube) << ", ";
            cout << edgeDatabaseA.check(cube) << ", ";
            cout << endDatabase.check(cube) << endl;
        }

        cout << endl;
    }
}

int main() {
    //test(15, 5);
    //generateDatabases();
    //testSymmetry();
    //testDatabases();
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    const int depth = 15;

    Cube cube = Cube();
    Solver solver = Solver(depth);

    scramble(cube, depth);

    //cube.print();
    //cout << endl;

    solver.multisolve(cube);
    _exit(0);
}

//IDEAS

// - Add few more symmetries for corners
// - Using symmetry try to use same edge database for other edges
// - Split Edge Database on multiple shards to improve loading and saving
// - Idea improve standardization by testing most significant bit first, not continuing if unpromising, also changing order position and orientation due to this.
// - Increase Size of Edge Database (amount of Edges tracked) thanks to savings due to symmetry and array storage, 8 Edges should be possible
// - Add more Types of Databases in the hope of improving heuristics. Full Edge Database for some moves? Permutation only database. Corner and Edge Combination database.
// - Improve Node Generation Speed (don't know exactly how. Smart maths or something)

