#pragma once

#include "database.h"

class EdgeDatabase : public Database<EdgeKey, EdgeKeyHash, size_t> {
public:
    EdgeDatabase(int depth, int i) : Database(depth), i(i) {
        name = "Edge " + to_string(i);
    };

protected:
    int i;

    string getPath() {
        return "databases/edge/database_" + to_string(i);
    };

    EdgeKey getKey(Cube& cube) {
        return cube.toEdgeKey(i);
    }
};