template <typename KeyType, typename Hash, typename HashType>
void Database<KeyType, Hash, HashType>::init() {
    cout << "Loading " << name << " Database..." << endl;

    bool result = load();
    if (result) {
        cout << "Loaded Database" << endl << endl;
        return;
    }

    cout << "Database not found" << endl << endl;
    cout << "Generating " << name << " Database..." << endl;

    Cube solvedCube = Cube();
    addEntry(solvedCube);

    multiFindChildren(solvedCube);
    save();

    cout << "Database generated" << endl;
    cout << "Collisions: " << collisions << endl << endl;
};

template <typename KeyType, typename Hash, typename HashType>
int Database<KeyType, Hash, HashType>::check(Cube& cube) {
    KeyType key = getKey(cube);
    HashType hash = Hash{}(key);
    const int shard = getShard(hash);

    auto& db = dbShards[shard];

    if (!db.count(key)) return -1;

    return db[key];
}

template <typename KeyType, typename Hash, typename HashType>
void Database<KeyType, Hash, HashType>::multiFindChildren(Cube cube) {
    vector<thread> threads;
    for (int i = 0; i < 6; i++) {
        for (int n = 1; n <= 3; n++) {
            threads.push_back(thread([this, cube, i, n]() {
                Cube newCube = cube;
                newCube.move(i, n);

                addEntry(newCube);

                findChildren(newCube, depthDB - 1, i);
                }));
        }
    }

    for (auto& t : threads) {
        t.join();
    }
};

template <typename KeyType, typename Hash, typename HashType>
void Database<KeyType, Hash, HashType>::findChildren(Cube cube, int depth, int lastMove) {
    depth -= 1;
    if (depth < 0) return;

    for (int i = 0; i < 6; i++) {
        if (i == lastMove) continue;
        if (i == 0 && lastMove == 1) continue;
        if (i == 3 && lastMove == 5) continue;
        if (i == 2 && lastMove == 4) continue;

        for (int n = 1; n <= 3; n++) {
            Cube newCube = cube;
            newCube.move(i, n);

            const bool collision = addEntry(newCube);
            if (collision) continue;

            findChildren(newCube, depth, i);
        }
    }
};

template <typename KeyType, typename Hash, typename HashType>
bool Database<KeyType, Hash, HashType>::addEntry(Cube cube) {
    KeyType key = getKey(cube);
    HashType hash = Hash{}(key);
    const int shard = getShard(hash);

    lock_guard<mutex> lock(shardMutexes[shard]);

    auto it = dbShards[shard].find(key);

    if (it == dbShards[shard].end()) {
        dbShards[shard][key] = static_cast<uint8_t>(cube.g);
        return false;
    }

    collisions++;

    if (it->second <= cube.g) return true;

    it->second = static_cast<uint8_t>(cube.g);
    return false;
}

template <typename KeyType, typename Hash, typename HashType>
void Database<KeyType, Hash, HashType>::save() {
    string path = getPath();
    filesystem::create_directories(path);

    for (int i = 0; i < numShards; i++) {
        ofstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);

        for (auto& [key, distance] : dbShards[i]) {
            file.write((char*)&key, sizeof(key));
            file.write((char*)&distance, sizeof(distance));
        }

        file.close();
    }
};

template <typename KeyType, typename Hash, typename HashType>
bool Database<KeyType, Hash, HashType>::load() {
    string path = getPath();
    if (!filesystem::is_directory(path)) return false;

    vector<thread> threads;
    for (int i = 0; i < numShards; i++) {
        threads.push_back(thread([this, i, path]() {
            ifstream file(path + "/shard_" + to_string(i) + ".bin", ios::binary);
            while (file.peek() != EOF) {
                KeyType key;
                if (!file.read((char*)&key, sizeof(key))) return;

                uint8_t distance;
                if (!file.read((char*)&distance, sizeof(distance))) return;

                dbShards[i][key] = distance;
            }
            }));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
};