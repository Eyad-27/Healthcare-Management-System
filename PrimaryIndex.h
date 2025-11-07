#pragma once
#include <bits/stdc++.h>
using namespace std;

class PrimaryIndex {
private:
    struct IndexEntry {
        string key;
        int rrn;
        bool deleted;
    };

    vector<IndexEntry> index;
    string indexFilename;
    list<int> availList;
    bool testingMode;

    int binarySearch(const string& key);

public:
    PrimaryIndex(const string& filename, bool testing = false)
        : indexFilename(filename), testingMode(testing) {
        loadIndex(filename);
    }

    void loadIndex(const string& filename);
    void saveIndex();

    int search(const string& key);
    void addEntry(const string& key, int rrn);
    void deleteEntry(const string& key);
};
