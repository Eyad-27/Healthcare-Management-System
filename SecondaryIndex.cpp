#include <bits/stdc++.h>
using namespace std;

#include "SecondaryIndex.h"


void SecondaryIndex::loadIndex(const string& filename) {
    indexHead = nullptr;
    ifstream file(filename);
    if (!file.is_open()) {
        ofstream newFile(filename);
        newFile.close();
        return;
    }

    string secondaryKey;
    string primaryKey;

    while (file >> secondaryKey >> primaryKey) {
        addEntry(indexHead, secondaryKey, primaryKey);
    }
    file.close();

}

void SecondaryIndex::saveIndex() {
    ofstream file(indexFilename);
    SecIndex* curr = indexHead;

    while (curr != nullptr) {
        SecNode* pkNode = curr->head;
        while (pkNode != nullptr) {
            file << curr->secondaryKey << " " << pkNode->primaryKey << endl;
            pkNode = pkNode->next;
        }
        curr = curr->next;
    }

    file.close();
}

vector<string> SecondaryIndex::search(const string& secondaryKey) {
    vector<string> primaryKeys;
    SecIndex* curr = indexHead;

    while (curr != nullptr && curr->secondaryKey < secondaryKey) {
        curr = curr->next;
    }

    if (curr != nullptr && curr->secondaryKey == secondaryKey) {
        SecNode* pkNode = curr->head;
        while (pkNode != nullptr) {
            primaryKeys.push_back(to_string(pkNode->primaryKey));
            pkNode = pkNode->next;
        }
    }

    return primaryKeys;
}

void SecondaryIndex::addEntry(SecIndex* &head, const string& secondaryKey, const string& primaryKey) {
    if (head == nullptr) {
        head = new SecIndex(secondaryKey);
        head->head = new SecNode(stoi(primaryKey));
        return;
    }

    SecIndex* curr = head;
    SecIndex* prev = nullptr;

    while (curr != nullptr && curr->secondaryKey < secondaryKey) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->secondaryKey == secondaryKey) {
        SecNode* pkNode = new SecNode(stoi(primaryKey));
        pkNode->next = curr->head;
        curr->head = pkNode;
    } else {
        SecIndex* newIndex = new SecIndex(secondaryKey);
        newIndex->head = new SecNode(stoi(primaryKey));
        if (prev == nullptr) {
            newIndex->next = head;
            head = newIndex;
        } else {
            newIndex->next = curr;
            prev->next = newIndex;
        }
    }

}

void SecondaryIndex::deleteEntry(SecIndex* &head, const string& secondaryKey, const string& primaryKey) {
    SecIndex* curr = head;
    SecIndex* prev = nullptr;

    while (curr != nullptr && curr->secondaryKey < secondaryKey) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->secondaryKey == secondaryKey) {
        SecNode* pkCurr = curr->head;
        SecNode* pkPrev = nullptr;
        int pkToDelete = stoi(primaryKey);

        while (pkCurr != nullptr && pkCurr->primaryKey != pkToDelete) {
            pkPrev = pkCurr;
            pkCurr = pkCurr->next;
        }

        if (pkCurr != nullptr) {
            if (pkPrev == nullptr) {
                curr->head = pkCurr->next;
            } else {
                pkPrev->next = pkCurr->next;
            }
            delete pkCurr;

            if (curr->head == nullptr) {
                if (prev == nullptr) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
            }
        }
    }

}
