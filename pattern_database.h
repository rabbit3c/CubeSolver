#pragma once
#include <unordered_map>
#include <mutex>
#include <vector>
#include "cube.h"

using CubeKey = array<uint32_t, 6>;

namespace std {
    template<>
    struct hash<CubeKey> {
        size_t operator()(const CubeKey& key) const noexcept {
            size_t seed = 0;

            for (uint32_t v : key)
                seed ^= std::hash<uint32_t>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
};

class PatternDatabase {
public:
    int depthDB;

    PatternDatabase(int depth);

    bool check(Cube& cube);

    int size();

private:
    static const int numShards = 32;
    unordered_map<CubeKey, vector<uint8_t>> dbShards[numShards];
    mutex shardMutexes[numShards];

    int getShard(const CubeKey& key);

    void multiFindChildren(Cube cube);
    void findChildren(Cube cube, int depth, int lastMove);
    void addEntry(Cube cube);

    void solve(Cube& cube, vector<uint8_t> moves);

    void save();
    bool load();
};