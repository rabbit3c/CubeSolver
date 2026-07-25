#pragma once

#include <cstdint>
#include <iostream>

using namespace std;

inline size_t splitMixHash(uint64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return static_cast<size_t>(x);
}

struct CubeKey {
    uint64_t corners;
    uint64_t edges;
    bool operator==(const CubeKey& other) const {
        return corners == other.corners && edges == other.edges;
    }
};

struct CubeKeyHash {
    size_t operator()(const CubeKey& cubeKey) const {
        size_t h1 = hash<uint64_t>{}(cubeKey.corners);
        size_t h2 = hash<uint64_t>{}(cubeKey.edges);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

struct CornerKey {
    uint64_t corners;
    bool operator==(const CornerKey& other) const {
        return corners == other.corners;
    }
};

struct CornerKeyHash {
    size_t operator()(const CornerKey& cornerKey) const {
        size_t h = hash<uint64_t>{}(cornerKey.corners);
        return h;
    }
};

struct EdgeKey {
    uint64_t edges;
    bool operator==(const EdgeKey& other) const {
        return edges == other.edges;
    }
};

struct EdgeKeyHash {
    size_t operator()(const EdgeKey& edgeKey) const {
        size_t h = splitMixHash(edgeKey.edges);
        return h;
    }
};