#include <random>
#include <sstream>
#include "cube.h"

using namespace std;

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

void executeMoves(Cube &cube, string strMoves) {
    vector<string> moves = split(strMoves);

    for (string move : moves) {
        char type = move[0];
        int n = parse(move);

        switch (move[0]) {
            case 'U':
                cube.up(n);
                break;
            case 'D':
                cube.down(n);
                break;
            case 'R':
                cube.right(n);
                break;
            case 'L':
                cube.left(n);
                break;
            case 'F':
                cube.front(n);
                break;
            case 'B':
                cube.back(n);
                break;
        }
    }
}

string scramble(Cube &cube, int numMoves) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist18(0, 17);

    string options[18]  = {"U", "U2", "U'", "D", "D2", "D'", "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'"};

    string moves;

    for (int _ = 0; _ < numMoves; _++) {
        int i = dist18(rng);
        moves.append(options[i] + " ");
    }

    executeMoves(cube, moves);

    return moves;
}

