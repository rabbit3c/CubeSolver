#include <random>
#include <sstream>
#include "cube.h"
//#include "databases/corner_database.h"
//#include "databases/edge_database.h"
//#include "databases/end_database.h"

int parse(string move) {
    if (move.back() == '\'') return 3;
    if (move.back() == '2') return 2;
    return 1;
}

vector<string> split(string str) {
    vector<string> result;
    stringstream ss(str);
    string word;

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

void executeMoves(Cube& cube, string strMoves) {
    vector<string> moves = split(strMoves);

    for (string move : moves) {
        const char type = move[0];
        const int n = parse(move);

        switch (move[0]) {
        case 'U':
            cube.move(0, n);
            break;
        case 'D':
            cube.move(1, n);
            break;
        case 'R':
            cube.move(5, n);
            break;
        case 'L':
            cube.move(3, n);
            break;
        case 'F':
            cube.move(2, n);
            break;
        case 'B':
            cube.move(4, n);
            break;
        }
    }
}

void scramble(Cube& cube, int numMoves, bool logging) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist6(0, 5);
    uniform_int_distribution<int> dist3(1, 3);

    //auto cornerDatabase = CornerDatabase(12);
    //cornerDatabase.init();
    //auto edgeDatabaseA = EdgeDatabase(12, 0);
    //edgeDatabaseA.init();
    //auto edgeDatabaseB = EdgeDatabase(12, 1);
    //edgeDatabaseB.init();
    //auto endDatabase = EndDatabase(8);
    //endDatabase.init();

    int lastI;
    for (int _ = 0; _ < numMoves; _++) {
        int i = dist6(rng);
        while (i == lastI) i = dist6(rng);
        lastI = i;

        const int n = dist3(rng);

        cube.move(i, n);
        //cout << cornerDatabase.check(cube) << endl;
        //cout << edgeDatabaseA.check(cube) << endl;
        //cout << edgeDatabaseB.check(cube) << endl;
        //cout << endDatabase.check(cube) << endl;
    }

    if (logging) {
        cout << "Cube scrambled" << endl;
        cout << "Scramble: " << endl;
        cube.printMoves();
        cout << endl;
    }

    cube.clearMoves();
    cube.g = 0;
}

