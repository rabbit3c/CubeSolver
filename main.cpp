#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"
#include "symmetry.h"

using namespace std;

void generateDatabases() {
    Symmetry::generateCornerSymmetries();
    {
        auto endDatabase = EndDatabase(7);
        endDatabase.init();
    }
    {
        auto cornerDatabase = CornerDatabase(12);
        cornerDatabase.init();
    }
    {
        auto edgeDatabase0 = EdgeDatabase(12, 0);
        edgeDatabase0.init();
    }
    {
        auto edgeDatabase1 = EdgeDatabase(12, 1);
        edgeDatabase1.init();
    }
}

void testSymmetry() {
    Symmetry::generateCornerSymmetries();

    for (int i = 0; i < 8; i++) {
        Cube cube = Cube();
        cube.move(2 + (i % 4), 1);
        cube.move(i / 4, 1);

        cout << cube.corners << endl;
        auto corners = Symmetry::standardizeCorners(cube.corners);
        cout << corners << endl;
    }
}

int main() {
    //test(15, 5);
    //generateDatabases();
    //testSymmetry();
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    Symmetry::generateCornerSymmetries();

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

// - Use Symmetry to have only one EdgeDatabase instead of two
// - Use Symmetry to reduce the size of the Databases (at least by factor 24, maybe 48)
// - Increase Size of End Database thanks to size reduction due to symmetry
// - Store Corner and Edge Databases as array instead of unordered map. Use the key as indices by transforming it into factorial
// - Increase Size of Edge Database (amount of Edges tracked) thanks to savings due to symmetry and array storage, 8 Edges should be possible
// - Add more Types of Databases in the hope of improving heuristics.
// - Improve Node Generation Speed (don't know exactly how. Smart maths or something)

