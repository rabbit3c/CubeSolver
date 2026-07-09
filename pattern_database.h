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
    unordered_map<size_t, uint8_t> dbShards[numShards];
    mutex shardMutexes[numShards];

    int getShard(const size_t& hash);

    void multiFindChildren(Cube cube);
    void findChildren(Cube cube, int depth, int lastMove);
    void addEntry(Cube cube);

    void solve(Cube& cube, vector<uint8_t> moves);

    void save();
    bool load();
};