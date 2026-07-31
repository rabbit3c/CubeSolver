#pragma once

#include "hash_database.h"

class EndDatabase : public HashDatabase<CubeKey, CubeKeyHash, size_t> {
public:
    EndDatabase(int depth) : HashDatabase(depth) {
        name = "End";
    };

protected:
    string getPath() {
        return "databases/end/database_d" + to_string(depthDB);
    };

    CubeKey getKey(Cube& cube) {
        return cube.toKey();
    }
};