#include "PrimaryIndex.h"

//--------------------------------------------------------
// Constructor — loads existing index file
//--------------------------------------------------------
PrimaryIndex::PrimaryIndex(const string& filename)
    : indexFilename(filename) {
    loadIndex();
}

//--------------------------------------------------------
// Load index from file
//--------------------------------------------------------
void PrimaryIndex::loadIndex() {
    index.clear();

    ifstream file(indexFilename);
    if (!file.is_open()) return;

    string key;
    long offset;
    char delMarker;

    while (file >> key >> offset >> delMarker) {
        bool isDeleted = (delMarker == '*');
        index.push_back({key, offset, isDeleted});
    }

    file.close();

    // Sort the index by key
    sort(index.begin(), index.end(), [](const IndexEntry& a, const IndexEntry& b) {
        return a.key < b.key;
    });
}

//--------------------------------------------------------
// Save index to file
//--------------------------------------------------------
void PrimaryIndex::saveIndex() {
    ofstream file(indexFilename, ios::trunc);

    for (auto& entry : index) {
        file << entry.key << " " << entry.offset << " "
             << (entry.deleted ? "*" : "-") << endl;
    }

    file.close();
}

//--------------------------------------------------------
// Binary Search
//--------------------------------------------------------
int PrimaryIndex::binarySearch(const string& key) {
    int left = 0, right = index.size() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (index[mid].key == key)
            return mid;

        if (index[mid].key < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

//--------------------------------------------------------
// Search (returns offset, or -1 if not found or deleted)
//--------------------------------------------------------
long PrimaryIndex::search(const string& key) {
    int pos = binarySearch(key);

    if (pos != -1 && !index[pos].deleted)
        return index[pos].offset;

    return -1;
}

//--------------------------------------------------------
// Add new entry (only stores key + offset)
//--------------------------------------------------------
bool PrimaryIndex::addEntry(const string& key, long offset) {
    // Check if already exists
    int pos = binarySearch(key);
    if (pos != -1 && !index[pos].deleted)
        return false; // duplicate

    index.push_back({key, offset, false});

    // Keep index sorted
    sort(index.begin(), index.end(), [](const IndexEntry& a, const IndexEntry& b) {
        return a.key < b.key;
    });

    return true;
}

//--------------------------------------------------------
// Delete entry (mark as deleted)
//--------------------------------------------------------
bool PrimaryIndex::deleteEntry(const string& key) {
    int pos = binarySearch(key);
    if (pos == -1)
        return false;

    index[pos].deleted = true;
    return true;

}

PrimaryIndex::PrimaryIndex() {
}


