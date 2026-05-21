#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

int main() {
    test(6, 10);
    return 0;

    SetConsoleOutputCP(CP_UTF8);

    int depth = 7;

    Cube cube = Cube();
    Solver solver = Solver(depth);
        
    scramble(cube, depth);
    //cube.print();

    cout << endl;

    solver.multisolve(cube);
}
