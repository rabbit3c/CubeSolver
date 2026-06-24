#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "pattern_database.h"

PatternDatabase::PatternDatabase(int depth) {
    depthDB = depth;

    cout << "Loading Pattern Database..." << endl;

    bool result = load();
    if (result) {
        cout << "Loaded Database" << endl << endl;
        return;
    }

    cout << "Database not found" << endl << endl;
    cout << "Generating Pattern Database..." << endl;

    Cube solvedCube = Cube();
    addEntry(solvedCube);

    multiFindChildren(solvedCube);
    save();

    cout << "Pattern Database generated" << endl << endl;
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

int PatternDatabase::getShard(const CubeKey& key) {
    return hash<CubeKey>{}(key) % numShards;
}

void PatternDatabase::addEntry(Cube cube) {
    CubeKey key = cube.toKey();
    const int shard = getShard(key);

    lock_guard<mutex> lock(shardMutexes[shard]);
    dbShards[shard][key] = cube.moves;
}

bool PatternDatabase::check(Cube& cube) {
    CubeKey key = cube.toKey();
    const int shard = getShard(key);

    auto& db = dbShards[shard];

    if (!db.count(key)) return false;

    solve(cube, db[key]);
    return true;
}

int PatternDatabase::size() {
    int totalSize = 0;
    int n = 0;
    for (auto shard : dbShards) {
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

void PatternDatabase::save() {
    string path = "databases/database_d" + to_string(depthDB);
    filesystem::create_directories(path);

    for (int i = 0; i < numShards; i++) {
        ofstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);

        for (auto& [key, moves] : dbShards[i]) {
            file.write((char*)key.data(), sizeof(CubeKey));
            size_t movesSize = moves.size();
            file.write((char*)&movesSize, sizeof(movesSize));
            file.write((char*)moves.data(), movesSize);
        }

        file.close();
    }
}

bool PatternDatabase::load() {
    string path = "databases/database_d" + to_string(depthDB);
    if (!filesystem::is_directory(path)) return false;

    vector<thread> threads;
    for (int i = 0; i < numShards; i++) {
        threads.push_back(thread([this, i, path]() {
            ifstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);
            while (file.peek() != EOF) {
                CubeKey key;
                file.read((char*)key.data(), sizeof(CubeKey));

                size_t movesSize;
                file.read((char*)&movesSize, sizeof(movesSize));
                vector<uint8_t> moves(movesSize);
                file.read((char*)moves.data(), movesSize);

                dbShards[i][key] = move(moves);
            }
            }));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
}

