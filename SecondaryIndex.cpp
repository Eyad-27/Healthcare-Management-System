#include <bits/stdc++.h>
using namespace std;

#include "SecondaryIndex.h"
int SecondaryIndex::binarySearch(const string& secondaryKey) {
    int left = 0;
    int right = indexes.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (indexes[mid].secondaryKey == secondaryKey) {
            return mid;
        } else if (indexes[mid].secondaryKey < secondaryKey) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return left; // Return insertion point if not found
}

void SecondaryIndex::loadIndex(const string& filename) {
    indexes.clear();
    availList = stack<int>();
    nextSlot = 0;

    ifstream file(filename);
    if (!file.is_open()) {
        ofstream newFile(filename);
        newFile.close();
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        SecIndex secIndex;
        ss >> secIndex.secondaryKey;
        int pkCount;
        ss >> pkCount;
        PKNode* head = nullptr;
        PKNode* tail = nullptr;
        for (int i = 0; i < pkCount; ++i) {
            string primaryKey;
            ss >> primaryKey;
            PKNode* newNode = new PKNode{primaryKey, nullptr};
            if (!head) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        secIndex.head = head;
        secIndex.slot = nextSlot++;
        indexes.push_back(secIndex);
    }
    file.close();
}

void SecondaryIndex::saveIndex() {
    ofstream file(indexFilename);
    for (const auto& secIndex : indexes) {
        file << secIndex.secondaryKey << " ";
        // Count primary keys
        int pkCount = 0;
        PKNode* curr = secIndex.head;
        while (curr) {
            pkCount++;
            curr = curr->next;
        }
        file << pkCount << " ";
        // Write primary keys
        curr = secIndex.head;
        while (curr) {
            file << curr->primaryKey << " ";
            curr = curr->next;
        }
        file << endl;
    }
    file.close();
}

vector<string> SecondaryIndex::search(const string& secondaryKey) {
    int pos = binarySearch(secondaryKey);
    vector<string> primaryKeys;
    if (pos < indexes.size() && indexes[pos].secondaryKey == secondaryKey) {
        PKNode *curr = indexes[pos].head;
        while (curr) {
            primaryKeys.push_back(curr->primaryKey);
            curr = curr->next;
        }
    }
    return primaryKeys;
}

void SecondaryIndex::addEntry(const string& secondaryKey, const string& primaryKey) {
    int pos = binarySearch(secondaryKey);
    if (pos < indexes.size() && indexes[pos].secondaryKey == secondaryKey) {
        PKNode* newNode = new PKNode{primaryKey, nullptr};
        PKNode *curr = indexes[pos].head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
        return;
    }
    PKNode* newNode = new PKNode{primaryKey, nullptr};
    SecIndex newSecIndex;
    newSecIndex.secondaryKey = secondaryKey;
    newSecIndex.head = newNode;
    if(!availList.empty()){
        newSecIndex.slot = availList.top();
        availList.pop();

    }else{
        newSecIndex.slot = nextSlot++;
    }
    indexes.insert(indexes.begin() + pos, newSecIndex);
}

void SecondaryIndex::deleteEntry(const string& secondaryKey, const string& primaryKey) {
    int pos = binarySearch(secondaryKey);
    if (pos < indexes.size() && indexes[pos].secondaryKey == secondaryKey) {
        PKNode *curr = indexes[pos].head;
        PKNode *prev = nullptr;
        while (curr) {
            if (curr->primaryKey == primaryKey) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    indexes[pos].head = curr->next;
                }
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        // If no more primary keys, remove the secondary index entry
        if (!indexes[pos].head) {
            availList.push(indexes[pos].slot);
            indexes.erase(indexes.begin() + pos);
        }
    }
}

SecondaryIndex::SecondaryIndex() {

}
