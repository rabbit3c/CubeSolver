#pragma once

#include <unordered_map>
#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "cube.h"

template <typename KeyType, typename HashType>
class Database {
public:
    int depthDB;

    Database(int depth) : depthDB(depth) {};

    void init() {
        cout << "Loading " << name << " Database..." << endl;

        bool result = load();
        if (result) {
            cout << "Loaded Database" << endl << endl;
            return;
        }

        cout << "Database not found" << endl << endl;
        cout << "Generating " << name << " Database..." << endl;

        Cube solvedCube = Cube();
        addEntry(solvedCube);

        multiFindChildren(solvedCube);
        save();

        cout << "Database generated" << endl;
        cout << "Collisions: " << collisions << endl << endl;
    };

    virtual int check(Cube& cube) {
        KeyType key = getKey(cube);
        size_t hash = HashType{}(key);
        const int shard = getShard(hash);

        auto& db = dbShards[shard];

        if (!db.count(key)) return -1;

        return db[key];
    }

protected:
    static const int numShards = 32;
    unordered_map<KeyType, uint8_t, HashType> dbShards[numShards];
    mutex shardMutexes[numShards];

    int collisions = 0;
    string name;

    int getShard(const size_t& hash) {
        return hash % numShards;
    };

    void multiFindChildren(Cube cube) {
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
    };

    void findChildren(Cube cube, int depth, int lastMove) {
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
    };

    virtual bool addEntry(Cube cube) {
        KeyType key = getKey(cube);
        size_t hash = HashType{}(key);
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

    void save() {
        string path = getPath();
        filesystem::create_directories(path);

        for (int i = 0; i < numShards; i++) {
            ofstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);

            for (auto& [key, distance] : dbShards[i]) {
                file.write((char*)&key, sizeof(key));
                file.write((char*)&distance, sizeof(distance));
            }

            file.close();
        }
    };

    bool load() {
        string path = getPath();
        if (!filesystem::is_directory(path)) return false;

        vector<thread> threads;
        for (int i = 0; i < numShards; i++) {
            threads.push_back(thread([this, i, path]() {
                ifstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);
                while (file.peek() != EOF) {
                    KeyType key;
                    if (!file.read((char*)&key, sizeof(key))) return;

                    uint8_t distance;
                    if (!file.read((char*)&distance, sizeof(distance))) return;

                    dbShards[i][key] = distance;
                }
                }));
        }

        for (auto& t : threads) {
            t.join();
        }

        return true;
    };

    virtual KeyType getKey(Cube& cube) = 0;
    virtual string getPath() = 0;
};