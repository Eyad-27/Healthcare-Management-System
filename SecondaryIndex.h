#pragma once
#include <bits/stdc++.h>
using namespace std;

// Secondary index maps secondary key -> one or more primary keys (via linked list or vector placeholder)
class SecondaryIndex {
private:
    struct SecNode{
        int primaryKey;
        SecNode* next;
        SecNode(int pk) : primaryKey(pk), next(nullptr) {}
    };
    struct SecIndex {
        string secondaryKey;
        SecNode* head; // Head of linked list of primary keys
        SecIndex* next;
        SecIndex(const string& sk) : secondaryKey(sk), head(nullptr), next(nullptr) {}
    };
    SecIndex* indexHead;
    string indexFilename;
public:
    SecondaryIndex(const string& filename)
        : indexFilename(filename), indexHead(nullptr) {
        loadIndex(filename);
    }
    void loadIndex(const string& filename);
    void saveIndex();
    vector<string> search(const string &secondaryKey);
    void deleteEntry(SecIndex *&head, const string &secondaryKey, const string &primaryKey);
    void addEntry(SecIndex *&head, const string &secondaryKey, const string &primaryKey);
};
