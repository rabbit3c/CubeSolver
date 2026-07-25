#pragma once

#include <unordered_map>
#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "../cube.h"

template <typename KeyType, typename Hash, typename HashType>
class Database {
public:
    int depthDB;

    Database(int depth) : depthDB(depth) {};

    void init();

    int check(Cube& cube);

protected:
    static const int numShards = 32;
    unordered_map<KeyType, uint8_t, Hash> dbShards[numShards];
    mutex shardMutexes[numShards];

    int collisions = 0;
    string name;

    int getShard(const HashType& hash) {
        return hash % numShards;
    };

    void multiFindChildren(Cube cube);
    void findChildren(Cube cube, int depth, int lastMove);

    bool addEntry(Cube cube);

    void save();
    bool load();

    virtual KeyType getKey(Cube& cube) = 0;
    virtual string getPath() = 0;
};

#include "database.tpp"