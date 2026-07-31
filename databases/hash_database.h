#pragma once

#include "database.h"

template <typename KeyType, typename Hash, typename HashType>
class HashDatabase : public Database<CubeKey> {
public:
    HashDatabase(int depth) : Database(depth) {}

    int check(Cube& cube) {
        KeyType key = getKey(cube);
        HashType hash = Hash{}(key);
        const int shard = getShard(hash);

        auto& db = dbShards[shard];

        if (!db.count(key)) return -1;

        return db[key];
    }

protected:
    static const int numShards = 32;
    unordered_map<KeyType, uint8_t, Hash> dbShards[numShards];
    mutex shardMutexes[numShards];

    int getShard(const HashType& hash) {
        return hash % numShards;
    };

    bool addEntry(Cube cube) {
        KeyType key = getKey(cube);
        HashType hash = Hash{}(key);
        const int shard = getShard(hash);

        lock_guard<mutex> lock(shardMutexes[shard]);

        auto it = dbShards[shard].find(key);

        if (it == dbShards[shard].end()) {
            dbShards[shard][key] = static_cast<uint8_t>(cube.g);
            return false;
        }

        collisions++;

        if (it->second <= cube.g) return true;

        it->second = static_cast<uint8_t>(cube.g);
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
    }

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
};