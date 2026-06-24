#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solver.h"

using namespace std;

//TODO: Change how depth is calculated in the solve function so that it just counts down
//TODO: Improve Database Lookup and Completion Checking, at the moment duplicate / useless
//TODO: Store Database in binary file instead of generating it every time

int main() {
    //test(13, 10);
    //return 0;

    SetConsoleOutputCP(CP_UTF8);

    int depth = 13;

    Cube cube = Cube();
    Solver solver = Solver(depth);
        
    scramble(cube, depth);
    //cube.print();

    cout << endl;

    solver.multisolve(cube);
}
