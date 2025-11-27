#include <bits/stdc++.h>
using namespace std;
#include "PrimaryIndex.h"

void testPrimaryIndex() {
    cout << "=== Testing Primary Index Implementation ===" << endl << endl;

    // ------------------ Test Doctors ------------------
    PrimaryIndex doctorsIndex("doctors_primary_index.txt", "doctors_data.txt", true);

    // Add test entries
    doctorsIndex.addEntry("D001", "D001|Dr. Alice|Cardiology");
    doctorsIndex.addEntry("D003", "D003|Dr. Bob|Neurology");
    doctorsIndex.addEntry("D002", "D002|Dr. Carol|Pediatrics");
    doctorsIndex.addEntry("D005", "D005|Dr. Eve|Dermatology");
    doctorsIndex.addEntry("D004", "D004|Dr. Frank|Oncology");

    cout << endl;

    // Test search operations
    cout << "Searching D002..." << endl;
    long offset = doctorsIndex.search("D002");
    if (offset != -1) cout << "Found D002 at offset: " << offset << endl;
    else cout << "D002 not found!" << endl;

    offset = doctorsIndex.search("D999");
    if (offset != -1) cout << "Found D999 at offset: " << offset << endl;
    else cout << "D999 not found (as expected)!" << endl;

    cout << endl << "Testing duplicate entry prevention..." << endl;
    doctorsIndex.addEntry("D001", "D001|Duplicate|Test"); // Should show error

    cout << endl << "Testing delete operations..." << endl;
    doctorsIndex.deleteEntry("D003"); // Delete an existing key
    doctorsIndex.deleteEntry("D999"); // Attempt to delete non-existent key

    cout << endl << "Testing AVAIL LIST reuse..." << endl;
    doctorsIndex.addEntry("D006", "D006|Dr. Grace|Orthopedics"); // Should reuse D003's offset

    doctorsIndex.saveIndex();

    cout << endl << "=== Doctors Testing Complete ===" << endl << endl;

    // ------------------ Test Appointments ------------------
    cout << "=== Testing Appointments Primary Index ===" << endl << endl;
    PrimaryIndex appointmentsIndex("appointments_primary_index.txt", "appointments_data.txt", true);

    // Add test appointments
    appointmentsIndex.addEntry("A001", "A001|D001|2025-11-28|10:00");
    appointmentsIndex.addEntry("A002", "A002|D002|2025-11-28|11:00");
    appointmentsIndex.addEntry("A003", "A003|D001|2025-11-29|09:00");
    appointmentsIndex.addEntry("A004", "A004|D003|2025-11-29|14:00");
    appointmentsIndex.addEntry("A005", "A005|D002|2025-11-30|12:00");

    cout << endl << "Searching A003..." << endl;
    offset = appointmentsIndex.search("A003");
    if (offset != -1) cout << "Found A003 at offset: " << offset << endl;
    else cout << "A003 not found!" << endl;

    cout << "Searching A999..." << endl;
    offset = appointmentsIndex.search("A999");
    if (offset != -1) cout << "Found A999 at offset: " << offset << endl;
    else cout << "A999 not found (as expected)!" << endl;

    cout << endl << "Testing delete operations on appointments..." << endl;
    appointmentsIndex.deleteEntry("A002"); // Delete an existing appointment
    appointmentsIndex.deleteEntry("A999"); // Attempt to delete non-existent appointment

    cout << endl << "Testing AVAIL LIST reuse on appointments..." << endl;
    appointmentsIndex.addEntry("A006", "A006|D003|2025-12-01|15:00"); // Should reuse A002's offset

    appointmentsIndex.saveIndex();

    cout << endl << "=== Appointments Testing Complete ===" << endl;
}

int main() {
    testPrimaryIndex();
    cout << "\nPress Enter to exit...";
    cin.get();
    return 0;
}

