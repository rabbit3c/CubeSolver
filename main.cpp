#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

//TODO: Change cube representation from sides and colors to numbers. One uint64_t for the edges, one for the corners. Including permutations

int main() {
    //test(15, 10);
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

//TODO: Start with low depth and increase it gradually until finding a solution

//TODO: Optimize Database Generation
//      Improve spreading on different shards, by improving hashing function
//      Use Symmetry
//      Reduce Max Depth

//TODO: Use fucking Symmetry

//TODO: Improve heuristic function.
//      Change to float and add numbers after the coma based on other heuristics. Don't fuck up the pruning

