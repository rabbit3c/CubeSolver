template <typename KeyType>
void Database<KeyType>::init() {
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

    cout << "Database generated" << endl;
    cout << "Collisions: " << collisions << endl << endl;
    cout << "Saving Database..." << endl;

    save();

    cout << "Database saved" << endl << endl;
};

template <typename KeyType>
void Database<KeyType>::multiFindChildren(Cube cube) {
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

template <typename KeyType>
void Database<KeyType>::findChildren(Cube cube, int depth, int lastMove) {
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