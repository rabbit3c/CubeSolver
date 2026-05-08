#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"
#include "solve.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Cube cube = Cube();

    int depth = 7;
    
    scramble(cube, depth);

    cout << endl;

    solve(cube, depth, 0);

    if (cube.completion()) cout << "Cube completed!" << endl;
}
