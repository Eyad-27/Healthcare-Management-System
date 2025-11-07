#include <bits/stdc++.h>
using namespace std;
#include "PrimaryIndex.h"

void testPrimaryIndex() {
    cout << "=== Testing Primary Index Implementation ===" << endl << endl;

    // Test Doctors Primary Index with testing mode enabled
    cout << "1. Testing Doctors Primary Index:" << endl;
    PrimaryIndex doctorsIndex("doctors_primary_index.txt", true); // true enables testing mode

    // Add test entries
    doctorsIndex.addEntry("D001", 100);
    doctorsIndex.addEntry("D003", 102);
    doctorsIndex.addEntry("D002", 101);
    doctorsIndex.addEntry("D005", 104);
    doctorsIndex.addEntry("D004", 103);

    cout << endl;

    // Test search operations
    cout << "2. Testing Search Operations:" << endl;
    int rrn = doctorsIndex.search("D002");
    if (rrn != -1) {
        cout << "Found D002 at RRN: " << rrn << endl;
    } else {
        cout << "D002 not found!" << endl;
    }

    rrn = doctorsIndex.search("D999");
    if (rrn != -1) {
        cout << "Found D999 at RRN: " << rrn << endl;
    } else {
        cout << "D999 not found (as expected)!" << endl;
    }
    cout << endl;

    // Test duplicate entry prevention
    cout << "3. Testing Duplicate Entry Prevention:" << endl;
    doctorsIndex.addEntry("D001", 200); // Should show error
    cout << endl;

    // Test delete operations
    cout << "4. Testing Delete Operations:" << endl;
    doctorsIndex.deleteEntry("D003"); // Delete an existing key
    doctorsIndex.deleteEntry("D999"); // Attempt to delete non-existent key
    cout << endl;

    // Add new entry to test AVAIL LIST reuse
    cout << "5. Testing AVAIL LIST reuse:" << endl;
    doctorsIndex.addEntry("D006", 105); // Should reuse RRN from D003
    cout << endl;

    // Save the index
    doctorsIndex.saveIndex();

    // Test Appointments Primary Index with testing mode enabled
    cout << "6. Testing Appointments Primary Index:" << endl;
    PrimaryIndex appointmentsIndex("appointments_primary_index.txt", true);

    appointmentsIndex.addEntry("A001", 500);
    appointmentsIndex.addEntry("A003", 502);
    appointmentsIndex.addEntry("A002", 501);

    cout << endl;

    // Test search in appointments
    rrn = appointmentsIndex.search("A002");
    if (rrn != -1) {
        cout << "Found A002 at RRN: " << rrn << endl;
    }

    // Delete one appointment and reuse RRN
    appointmentsIndex.deleteEntry("A003");
    appointmentsIndex.addEntry("A004", 503); // Should reuse RRN from A003

    // Save appointments index
    appointmentsIndex.saveIndex();

    cout << endl << "=== Testing Complete ===" << endl;
}

int main() {
    testPrimaryIndex();

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}
