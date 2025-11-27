#pragma once
#include <bits/stdc++.h>
using namespace std;

// Secondary index maps secondary key -> one or more primary keys (via linked list)
class SecondaryIndex {
private:
    struct PKNode {
        string primaryKey;
        PKNode* next;
    };
    struct SecIndex {
        string secondaryKey;
        PKNode* head;
        int slot = -1;
    };

    vector <SecIndex> indexes;
    string indexFilename;
    stack<int> availList;
    int nextSlot = 0;
    int binarySearch(const string& secondaryKey);
public:
    SecondaryIndex(const string& filename)
        : indexFilename(filename) {
        loadIndex(filename);
    }
    void loadIndex(const string& filename);
    void saveIndex();
    vector<string> search(const string &secondaryKey);
    void addEntry(const string &secondaryKey, const string &primaryKey);
    void deleteEntry(const string &secondaryKey, const string &primaryKey);

    SecondaryIndex();
};
