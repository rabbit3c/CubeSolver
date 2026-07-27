#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"
#include "symmetry.h"

using namespace std;

void generateDatabases() {
    Symmetry::generate();
    {
        auto endDatabase = EndDatabase(8);
        endDatabase.init();
    }
    {
        auto cornerDatabase = CornerDatabase(12);
        cornerDatabase.init();
    }
    {
        auto edgeDatabase = EdgeDatabase(12, 0);
        edgeDatabase.init();
    }
    {
        auto edgeDatabase = EdgeDatabase(12, 1);
        edgeDatabase.init();
    }
}

void testSymmetry() {
    Symmetry::generate();

    for (int i = 0; i < 8; i++) {
        Cube cube = Cube();
        cube.move(2 + (i % 4), 1);
        cube.move(i / 4, 1);

        cout << "Move " << i + 1 << ": " << endl;

        cout << cube.corners << " -> ";
        auto corners = Symmetry::standardizeCorners(cube.corners);
        cout << corners << endl;

        cout << cube.edges << " -> ";
        auto edges = Symmetry::standardizeEdges(cube.edges);
        cout << edges << endl << endl;
    }
}

int main() {
    //test(15, 5);
    //generateDatabases();
    //testSymmetry();
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    Symmetry::generate();

    const int depth = 16;

    Cube cube = Cube();
    Solver solver = Solver(depth);

    scramble(cube, depth);

    //cube.print();
    //cout << endl;

    solver.multisolve(cube);
    _exit(0);
}

//IDEAS

// - Use Symmetry to have only one EdgeDatabase instead of two
// - Use Symmetry to reduce the size of the Databases (at least by factor 24, maybe 48)
// - Increase Size of End Database thanks to size reduction due to symmetry
// - Store Corner and Edge Databases as array instead of unordered map. Use the key as indices by transforming it into factorial
// - Increase Size of Edge Database (amount of Edges tracked) thanks to savings due to symmetry and array storage, 8 Edges should be possible
// - Add more Types of Databases in the hope of improving heuristics. Full Edge Database for some moves? Permutation only database. Corner and Edge Combination database.
// - Improve Node Generation Speed (don't know exactly how. Smart maths or something)

