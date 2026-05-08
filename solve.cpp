#include <vector>
#include "cube.h"

void solve(Cube cube, int maxDepth, int depth) {
    depth += 1;
    if (depth > maxDepth) return;

    vector<void(Cube::*)(int)> options = {
        &Cube::up, &Cube::down, &Cube::right, &Cube::left, &Cube::front, &Cube::back
    };

    vector<Cube> cubes;
    for (void(Cube::*option)(int) : options) {
        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            (newCube.*option)(n);

            if (newCube.completion()) {
                cout << "Cube solved" << endl;
                cout << "Solution: " << endl;
                newCube.printMoves();
                
                return;
            }
            
            solve(newCube, maxDepth, depth);
        }
    }
}