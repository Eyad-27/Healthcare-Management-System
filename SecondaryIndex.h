#pragma once
#include <bits/stdc++.h>
using namespace std;

// Secondary index maps secondary key -> one or more primary keys (via linked list or vector placeholder)
class SecondaryIndex {
public:
    void loadIndex(const string& filename);
    void saveIndex();
    vector<string> search(const string& secondaryKey);
    void addEntry(const string& secondaryKey, const string& primaryKey);
    void deleteEntry(const string& secondaryKey, const string& primaryKey);
};
