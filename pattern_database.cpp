#include "pattern_database.h"
#include <thread>
#include <mutex>

PatternDatabase::PatternDatabase(int depth) {
    cout << "Generating Pattern Database..." << endl;

    depthDB = depth;

    Cube solvedCube = Cube();
    addEntry(solvedCube);

    multiFindChildren(solvedCube);

    cout << "Pattern Database generated" << endl;
    //cout << size() << " Entries" << endl;
    cout << endl;
}

void PatternDatabase::multiFindChildren(Cube cube) {
    vector<thread> threads;
    for (int i = 0; i < 6; i++) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([this, cube, i, n]() {
                Cube newCube = cube;
                newCube.move(i, n);

                addEntry(newCube);
                
                findChildren(newCube, depthDB - 1, i);
            }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}


void PatternDatabase::findChildren(Cube cube, int depth, int lastMove) {
    depth -= 1;
    if (depth < 0) return;

    for (int i = 0; i < 6; i++) {
        if (i == lastMove) continue;
        if (i == 1 && lastMove == 0) continue;
        if (i == 5 && lastMove == 3) continue;
        if (i == 4 && lastMove == 2) continue;

        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            newCube.move(i, n);
            
            addEntry(newCube);

            findChildren(newCube, depth, i);
        }
    }
}

void PatternDatabase::addEntry(Cube cube) {
    CubeKey key = cube.toKey();
    const int shard = getShard(key);

    lock_guard<mutex> lock(shard_mutexes[shard]);
    db_shards[shard][key] = cube.moves;
}

bool PatternDatabase::check(Cube& cube) {
    CubeKey key = cube.toKey();
    const int shard = getShard(key);
    
    auto& db = db_shards[shard];

    if (!db.count(key)) return false;

    solve(cube, db[key]);
    return true;
}

int PatternDatabase::size() {
    int totalSize = 0;
    int n = 0;
    for (auto shard : db_shards) {
        //cout << "Shard " << n << ": " << shard.size() << endl;
        totalSize += shard.size();
        n++;
    }
    return totalSize;
}

void PatternDatabase::solve(Cube& cube, vector<uint8_t> moves) {
    for (int j = moves.size() - 1; j >= 0; j--) {
        uint8_t move = moves[j];
        const int i = move / 10;
        const int n = move % 10;
        
        move = i * 10 + (4 - n);
        cube.moves.push_back(move);
    }

    cube.cube = Cube().cube;
}
