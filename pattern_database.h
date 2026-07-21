#pragma once

#include <unordered_map>
#include <mutex>
#include <vector>
#include "cube.h"

class PatternDatabase {
public:
    int depthDB;

    PatternDatabase(int depth);

    int check(Cube& cube);

    int size();

private:
    static const int numShards = 32;
    unordered_map<CubeKey, uint8_t, CubeKeyHash> dbShards[numShards];
    mutex shardMutexes[numShards];

    int collisions = 0;

    int getShard(const size_t& hash);

    void multiFindChildren(Cube cube);
    void findChildren(Cube cube, int depth, int lastMove);
    bool addEntry(Cube cube);

    void save();
    bool load();
};