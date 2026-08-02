#pragma once

#include "database.h"
#include <atomic>
#include <cstring>

class DenseDatabase : public Database<int> {
public:
    DenseDatabase(int depth, int size) : Database(depth), db(size) {
        static_assert(sizeof(atomic<uint8_t>) == sizeof(uint8_t));
        static_assert(atomic<uint8_t>::is_always_lock_free);

        memset(reinterpret_cast<uint8_t*>(db.data()), 255, db.size());
    }

    int check(Cube& cube) {
        int key = getKey(cube);

        return db[key];
    }

protected:
    vector<atomic<uint8_t>> db;

    bool addEntry(Cube cube) {
        int key = getKey(cube);

        uint8_t value = db[key];
        if (value == 255) {
            db[key] = static_cast<uint8_t>(cube.g);
            return false;
        }

        collisions++;

        if (value <= cube.g) return true;

        db[key] = cube.g;
        return false;
    }

    void save() {
        string path = getPath();
        filesystem::create_directories(path);

        ofstream file(path + "/shard.bin", ios::binary);

        for (auto& distance : db) {
            file.write((char*)&distance, sizeof(distance));
        }

        file.close();
    }

    bool load() {
        string path = getPath();
        if (!filesystem::is_directory(path)) return false;

        ifstream file(path + "/shard.bin", ios::binary);
        if (!file) return false;

        file.read(reinterpret_cast<char*>(rawBytePointer(db)), db.size());
        return true;
    };

    uint8_t* rawBytePointer(vector<atomic<uint8_t>>& db) {
        return reinterpret_cast<uint8_t*>(db.data());
    }

    int factorial(int n) {
        if (n < 0) throw invalid_argument("n too small");;

        int f = 1;
        for (int i = 1; i <= n; i++) {
            f *= i;
        }
        return f;
    }
};