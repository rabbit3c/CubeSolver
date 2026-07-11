#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

//TODO: Evaluate different cube states

int main() {
    //test(13, 10);
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    const int depth = 13;

    Cube cube = Cube();
    Solver solver = Solver(depth);

    scramble(cube, depth);
    //cube.print();
    //float h = cube.h();
    //cout << "H: " << h << endl;

    //cout << endl;

    solver.multisolve(cube);
    _exit(0);
}
