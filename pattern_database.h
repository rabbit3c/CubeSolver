#pragma once
#include <unordered_map>
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

    private:
        unordered_map<CubeKey, vector<uint8_t>> db;

        static vector<void(Cube::*)(int)> options;

        void findChildren(Cube cube, int depth, int lastMove);
        void addEntry(Cube cube);

        void solve(Cube& cube);
};