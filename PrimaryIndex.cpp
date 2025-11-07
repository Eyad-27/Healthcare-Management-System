#include "PrimaryIndex.h"

int PrimaryIndex::binarySearch(const string& key) {
    int left = 0;
    int right = index.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (index[mid].key == key && !index[mid].deleted) {
            return mid;
        } else if (index[mid].key < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

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
    int rrn;
    char delMarker;

    while (file >> key >> rrn >> delMarker) {
        bool isDeleted = (delMarker == '*');
        index.push_back({key, rrn, isDeleted});
        if (isDeleted) availList.push_back(rrn);
    }

    file.close();
    sort(index.begin(), index.end(), [](const auto &a, const auto &b){
        return a.key < b.key;
    });
}

void PrimaryIndex::saveIndex() {
    ofstream file(indexFilename);
    for (const auto& entry : index) {
        file << entry.key << " " << entry.rrn << " " << (entry.deleted ? "*" : "-") << endl;
    }
    file.close();
}

int PrimaryIndex::search(const string& key) {
    int pos = binarySearch(key);
    if (pos != -1) return index[pos].rrn;
    return -1;
}

void PrimaryIndex::addEntry(const string& key, int rrn) {
    if (search(key) != -1) {
        cout << "Error: Key '" << key << "' already exists in primary index!" << endl;
        return;
    }

    if (!availList.empty()) {
        int reusedRRN = availList.front();
        availList.pop_front();
        index.push_back({key, reusedRRN, false});
        cout << "Successfully added key '" << key << "' at reused RRN " << reusedRRN << " to primary index." << endl;
    } else {
        index.push_back({key, rrn, false});
        cout << "Successfully added key '" << key << "' with RRN " << rrn << " to primary index." << endl;
    }

    sort(index.begin(), index.end(), [](const auto &a, const auto &b){
        return a.key < b.key;
    });
}

void PrimaryIndex::deleteEntry(const string& key) {
    int pos = binarySearch(key);
    if (pos == -1) {
        cout << "Warning: Key '" << key << "' not found in primary index. Cannot delete." << endl;
        return;
    }

    index[pos].deleted = true;
    availList.push_back(index[pos].rrn);
    cout << "Successfully deleted key '" << key << "' from primary index. RRN added to AVAIL LIST." << endl;
}
