#pragma once
#include <bits/stdc++.h>
using namespace std;

// Primary index maps primary key -> RRN
class PrimaryIndex {
public:
    void loadIndex(const string& filename);
    void saveIndex();
    int search(const string& key);
    void addEntry(const string& key, int rrn);
    void deleteEntry(const string& key);
};
