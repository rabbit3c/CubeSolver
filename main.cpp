#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Cube cube = Cube();

    int depth = 6;
    
    scramble(cube, depth);

    cout << endl;

    Solver::multisolve(cube, depth);

    if (cube.completion()) cout << "Cube completed!" << endl;
}
