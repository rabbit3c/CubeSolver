#include <iostream>
#include <windows.h>
#include "cube.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Cube cube = Cube();
    cube.print();
}
