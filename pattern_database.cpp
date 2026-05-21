#include "pattern_database.h"

vector<void(Cube::*)(int)> PatternDatabase::options = {
    &Cube::up, &Cube::down, &Cube::right, &Cube::left, &Cube::front, &Cube::back
};

PatternDatabase::PatternDatabase(int depth) {
    cout << "Generating Pattern Database..." << endl;

    depthDB = depth;

    Cube solvedCube = Cube();
    addEntry(solvedCube);
    findChildren(solvedCube, depth, -1);

    cout << "Pattern Database generated" << endl << endl;
}

void PatternDatabase::findChildren(Cube cube, int depth, int lastMove) {
    depth -= 1;
    if (depth < 0) return;

    for (int i = 0; i < options.size(); i++) {
        if (i == lastMove) continue;
        if (i + 1 == lastMove && i % 2 == 0) continue;

        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            (newCube.*options[i])(n);
            
            addEntry(newCube);

            findChildren(newCube, depth, i);
        }
    }
}

void PatternDatabase::addEntry(Cube cube) {
    db[cube.toKey()] = cube.moves;
}

bool PatternDatabase::check(Cube& cube) {
    if (!db.count(cube.toKey())) return false;

    solve(cube);
    return true;
}

void PatternDatabase::solve(Cube& cube) {
    vector<uint8_t> moves = db[cube.toKey()];
    for (int j = moves.size() - 1; j >= 0; j--) {
        uint8_t move = moves[j];
        int i = move / 10;
        int n = move % 10;
        
        move = i * 10 + (4 - n);
        cube.moves.push_back(move);
    }

    cube.cube = Cube().cube;
}
