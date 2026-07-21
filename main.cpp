#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

//TODO: Change cube representation from sides and colors to numbers. One uint64_t for the edges, one for the corners. Including permutations

int main() {
    //test(13, 10);
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    const int depth = 14;

    //TODO: There is some bug. Try to figure out what is the problem. Probably incorrectly playing the moves I would imagine

    Cube cube = Cube();
    Solver solver = Solver(depth);

    scramble(cube, depth);

    //cube.print();
    //float h = cube.h();
    //cout << "H: " << h << endl;

    //cout << endl;

    //solver.solve(cube, depth, -1);
    solver.multisolve(cube);
    _exit(0);
}
