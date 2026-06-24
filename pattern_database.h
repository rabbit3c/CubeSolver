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
        static const int num_shards = 32;
        unordered_map<CubeKey, vector<uint8_t>> db_shards[num_shards];
        mutex shard_mutexes[num_shards];

        int getShard(const CubeKey& key) {
            return hash<CubeKey>{}(key) % num_shards;
        }

        void multiFindChildren(Cube cube);
        void findChildren(Cube cube, int depth, int lastMove);
        void addEntry(Cube cube);

        void solve(Cube& cube, vector<uint8_t> moves);
};