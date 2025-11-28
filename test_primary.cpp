#include <bits/stdc++.h>
using namespace std;
#include "PrimaryIndex.h"

void testPrimaryIndex() {
    cout << "=== Testing Primary Index ===\n\n";

    // ------------------ Test Doctors Index ------------------
    PrimaryIndex doctorsIndex("doctors_primary_index.txt");

    // Add entries (using dummy offsets)
    cout << "Adding doctor entries...\n";
    doctorsIndex.addEntry("D001", 0);
    doctorsIndex.addEntry("D003", 1);
    doctorsIndex.addEntry("D002", 2);
    doctorsIndex.addEntry("D005", 3);
    doctorsIndex.addEntry("D004", 4);

    // Test search
    cout << "\nSearching for D002...\n";
    long offset = doctorsIndex.search("D002");
    if (offset != -1) cout << "Found D002 at offset: " << offset << endl;
    else cout << "D002 not found!" << endl;

    cout << "Searching for D999...\n";
    offset = doctorsIndex.search("D999");
    if (offset != -1) cout << "Found D999 at offset: " << offset << endl;
    else cout << "D999 not found (as expected)!" << endl;

    // Test duplicate prevention
    cout << "\nTesting duplicate entry prevention...\n";
    if (!doctorsIndex.addEntry("D001", 5))
        cout << "Duplicate D001 not added (correct)\n";

    // Test deletion
    cout << "\nDeleting D003...\n";
    if (doctorsIndex.deleteEntry("D003"))
        cout << "D003 marked as deleted\n";
    if (!doctorsIndex.deleteEntry("D999"))
        cout << "D999 does not exist (cannot delete)\n";

    // Save index
    doctorsIndex.saveIndex();
    cout << "\nDoctors index saved.\n";

    // ------------------ Test Appointments Index ------------------
    PrimaryIndex appointmentsIndex("appointments_primary_index.txt");

    // Add entries (dummy offsets)
    cout << "\nAdding appointment entries...\n";
    appointmentsIndex.addEntry("A001", 0);
    appointmentsIndex.addEntry("A002", 1);
    appointmentsIndex.addEntry("A003", 2);
    appointmentsIndex.addEntry("A004", 3);
    appointmentsIndex.addEntry("A005", 4);

    // Search appointments
    cout << "\nSearching for A003...\n";
    offset = appointmentsIndex.search("A003");
    if (offset != -1) cout << "Found A003 at offset: " << offset << endl;
    else cout << "A003 not found!" << endl;

    // Delete test
    cout << "\nDeleting A002...\n";
    if (appointmentsIndex.deleteEntry("A002"))
        cout << "A002 marked as deleted\n";

    appointmentsIndex.saveIndex();
    cout << "\nAppointments index saved.\n";

    cout << "\n=== Testing Complete ===\n";
}

int main() {
    testPrimaryIndex();
    cout << "\nPress Enter to exit...";
    cin.get();
    return 0;
}
