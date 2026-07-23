#pragma once

#include "database.h"

class CornerDatabase : public Database<CornerKey, CornerKeyHash> {
public:
    CornerDatabase(int depth) : Database(depth) {
        name = "Corner";
    };

protected:
    string getPath() {
        return "databases/corner/database";
    };

    CornerKey getKey(Cube& cube) {
        return cube.toCornerKey();
    }
};