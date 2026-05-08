#include <iostream>
#include <windows.h>
#include "cube.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Cube cube = Cube();
    cube.right();
    cube.up();
    cube.down(2);
    cube.left(3);
    cube.front(1);
    cube.right(2);
    cube.back(2);

    cube.print();

    if (cube.completion()) cout << "Cube completed!" << endl;
}
