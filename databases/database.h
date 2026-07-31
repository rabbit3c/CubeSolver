#pragma once

#include <unordered_map>
#include <mutex>
#include <thread>
#include <fstream>
#include <filesystem>
#include "../cube.h"

template <typename KeyType>
class Database {
public:
    int depthDB;

    Database(int depth) : depthDB(depth) {};

    void init();

    virtual int check(Cube& cube) = 0;

protected:
    int collisions = 0;
    string name;

    void multiFindChildren(Cube cube);
    void findChildren(Cube cube, int depth, int lastMove);

    virtual bool addEntry(Cube cube) = 0;

    virtual void save() = 0;
    virtual bool load() = 0;

    virtual KeyType getKey(Cube& cube) = 0;
    virtual string getPath() = 0;
};

#include "database.tpp"