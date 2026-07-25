#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

void generateDatabases() {
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

int main() {
    //test(15, 5);
    //return 0;

    //generateDatabases();
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    const int depth = 15;

    Cube cube = Cube();
    Solver solver = Solver(depth);

    scramble(cube, depth);

    //cube.print();
    //float h = cube.h();
    //cout << "Corners: " << cube.corners << endl;
    //cout << "Edges: " << cube.edges << endl;
    //cout << "H: " << cube.evaluate() << endl;

    //cout << endl;

    solver.multisolve(cube);
    _exit(0);
}


//TODO: Start with low depth and increase it gradually until finding a solution V

//TODO: Optimize Database Generation
//      Improve spreading on different shards, by improving hashing function V
//      Use Symmetry
//      Reduce Max Depth V

//TODO: Use fucking Symmetry

//TODO: Improve heuristic function.
//      Change to float and add numbers after the coma based on other heuristics. Don't fuck up the pruning

//TODO: Store only the distance at the the correct place (corresponding to the key) instead of storing key and distance;

