#pragma once

#include "database.h"

class EndDatabase : public Database<CubeKey, CubeKeyHash, size_t> {
public:
    EndDatabase(int depth) : Database(depth) {
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