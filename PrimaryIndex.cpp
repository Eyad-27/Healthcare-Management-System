#include "PrimaryIndex.h"

// ------------------------- Binary Search -------------------------
int PrimaryIndex::binarySearch(const string& key) {
    int left = 0;
    int right = index.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (index[mid].key == key && !index[mid].deleted) return mid;
        else if (index[mid].key < key) left = mid + 1;
        else right = mid - 1;
    }

    return -1;
}

// ------------------------- Load Index -------------------------
void PrimaryIndex::loadIndex(const string& filename) {
    index.clear();
    availList.clear();

    if (testingMode) {
        ofstream clearFile(filename, ios::trunc);
        clearFile.close();
    }

    ifstream file(filename);
    if (!file.is_open()) {
        ofstream newFile(filename);
        newFile.close();
        return;
    }

    string key;
    long offset;
    int length;
    char delMarker;

    while (file >> key >> offset >> length >> delMarker) {
        bool isDeleted = (delMarker == '*');
        index.push_back({key, offset, length, isDeleted});
        if (isDeleted) availList.push_back({offset, length});
    }

    file.close();

    sort(index.begin(), index.end(), [](const auto &a, const auto &b){
        return a.key < b.key;
    });
}

// ------------------------- Save Index -------------------------
void PrimaryIndex::saveIndex() {
    ofstream file(indexFilename);
    for (const auto& entry : index) {
        file << entry.key << " " << entry.offset << " " << entry.length
             << " " << (entry.deleted ? "*" : "-") << endl;
    }
    file.close();
}

// ------------------------- Search -------------------------
long PrimaryIndex::search(const string& key) {
    int pos = binarySearch(key);
    if (pos != -1) return index[pos].offset;
    return -1;
}

// ------------------------- Add Entry -------------------------
void PrimaryIndex::addEntry(const string& key, const string& record) {
    if (search(key) != -1) {
        cout << "Error: Key '" << key << "' already exists!" << endl;
        return;
    }

    fstream file(dataFilename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        file.open(dataFilename, ios::out | ios::binary);
        file.close();
        file.open(dataFilename, ios::in | ios::out | ios::binary);
    }

    long writeOffset = 0;
    int recordLength = record.size() + 1; // include newline

    // Try to reuse a deleted record space that fits
    bool reused = false;
    for (auto it = availList.begin(); it != availList.end(); ++it) {
        if (it->second >= recordLength) { // space is large enough
            writeOffset = it->first;
            availList.erase(it);
            file.seekp(writeOffset);
            reused = true;
            cout << "Reused deleted record space at offset " << writeOffset << endl;
            break;
        }
    }

    if (!reused) { // append to end
        file.seekp(0, ios::end);
        writeOffset = file.tellp();
        cout << "Added new record at offset " << writeOffset << endl;
    }

    string recordWithMarker = record + "\n";
    file.write(recordWithMarker.c_str(), recordWithMarker.size());
    file.close();

    index.push_back({key, writeOffset, recordLength, false});
    sort(index.begin(), index.end(), [](const auto &a, const auto &b){
        return a.key < b.key;
    });
}

// ------------------------- Delete Entry -------------------------
void PrimaryIndex::deleteEntry(const string& key) {
    int pos = binarySearch(key);
    if (pos == -1) {
        cout << "Warning: Key '" << key << "' not found!" << endl;
        return;
    }

    index[pos].deleted = true;
    availList.push_back({index[pos].offset, index[pos].length});

    fstream file(dataFilename, ios::in | ios::out | ios::binary);
    file.seekp(index[pos].offset);
    file.put('*'); // mark as deleted
    file.close();

    cout << "Deleted key '" << key << "'. Offset added to AVAIL LIST." << endl;
}

PrimaryIndex::PrimaryIndex() {

}
