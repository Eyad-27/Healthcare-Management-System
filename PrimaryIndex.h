#pragma once
#include <bits/stdc++.h>
using namespace std;

class PrimaryIndex {
private:
    struct IndexEntry {
        string key;
        long offset;
        int length;
        bool deleted;
    };

    vector<IndexEntry> index;
    string dataFilename;
    string indexFilename;
    list<pair<long, int>> availList;
    bool testingMode;

    int binarySearch(const string& key);

public:
    PrimaryIndex(const string& filename, const string& dataFile, bool testing = false)
        : indexFilename(filename), dataFilename(dataFile), testingMode(testing) {
        loadIndex(filename);
    }

    void loadIndex(const string& filename);
    void saveIndex();

    long search(const string& key);
    void addEntry(const string& key, const string& record);
    void deleteEntry(const string& key);
};
