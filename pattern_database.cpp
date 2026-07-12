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
    cout << "Collision: " << collisions << endl << endl;
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
        if (i == 0 && lastMove == 1) continue;
        if (i == 3 && lastMove == 5) continue;
        if (i == 2 && lastMove == 4) continue;

        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            newCube.move(i, n);

            const bool collision = addEntry(newCube);
            if (collision) continue;

            findChildren(newCube, depth, i);
        }
    }
}

int PatternDatabase::getShard(const size_t& hash) {
    return hash % numShards;
}

bool PatternDatabase::addEntry(Cube cube) {
    size_t hash = cube.toHash();
    const int shard = getShard(hash);

    lock_guard<mutex> lock(shardMutexes[shard]);

    if (!dbShards[shard].count(hash)) {
        dbShards[shard][hash] = cube.distance;
        return false;
    }

    collisions++;

    if (dbShards[shard][hash] <= cube.distance) return true;

    dbShards[shard][hash] = cube.distance;
    return false;
}

int PatternDatabase::check(Cube& cube) {
    size_t hash = cube.toHash();
    const int shard = getShard(hash);

    auto& db = dbShards[shard];

    if (!db.count(hash)) return -1;

    return db[hash];
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

        for (auto& [key, distance] : dbShards[i]) {
            file.write((char*)&key, sizeof(size_t));
            file.write((char*)&distance, sizeof(distance));
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
                size_t key;
                file.read((char*)&key, sizeof(size_t));

                uint8_t distance;
                file.read((char*)&distance, sizeof(distance));

                dbShards[i][key] = distance;
            }
            }));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
}

