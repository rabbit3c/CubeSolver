#include <iostream>
#include <windows.h>
#include "cube.h"
#include "scrambler.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Cube cube = Cube();
    
    string moves = scramble(cube, 4);
    cout << moves << endl;

    cube.print();

    if (cube.completion()) cout << "Cube completed!" << endl;
}
