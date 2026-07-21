#include <thread>
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
    CubeKey key = cube.toKey();
    size_t hash = CubeKeyHash{}(key);
    const int shard = getShard(hash);

    lock_guard<mutex> lock(shardMutexes[shard]);

    auto it = dbShards[shard].find(key);

    if (it == dbShards[shard].end()) {
        dbShards[shard][key] = cube.distance;
        return false;
    }

    collisions++;

    if (it->second <= cube.distance) return true;

    it->second = cube.distance;
    return false;
}

int PatternDatabase::check(Cube& cube) {
    CubeKey key = cube.toKey();
    size_t hash = CubeKeyHash{}(key);
    const int shard = getShard(hash);

    auto& db = dbShards[shard];

    if (!db.count(key)) return -1;

    return db[key];
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

void PatternDatabase::save() {
    string path = "databases/database_d" + to_string(depthDB);
    filesystem::create_directories(path);

    for (int i = 0; i < numShards; i++) {
        ofstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);

        for (auto& [key, distance] : dbShards[i]) {
            file.write((char*)&key, sizeof(key));
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
                CubeKey key;
                if (!file.read((char*)&key, sizeof(key))) break;

                uint8_t distance;
                if (!file.read((char*)&distance, sizeof(distance))) break;

                dbShards[i][key] = distance;
            }
            }));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
}

