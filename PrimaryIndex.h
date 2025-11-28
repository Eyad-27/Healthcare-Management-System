#pragma once
#include <bits/stdc++.h>
using namespace std;

class PrimaryIndex {
private:
    struct IndexEntry {
        string key;
        long offset;
        bool deleted;
    };

    vector<IndexEntry> index;
    string indexFilename;

    int binarySearch(const string& key);

public:
    PrimaryIndex(const string& filename);

    void loadIndex();
    void saveIndex();

    long search(const string& key);
    bool addEntry(const string& key, long offset);
    bool deleteEntry(const string& key);

    PrimaryIndex();
};
