#include <bits/stdc++.h>
using namespace std;
#include "DataStructs.h"
#include "FileManager.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "Operations.h"


static void displayMenu() {
    cout << "\n==== Healthcare Management System ====\n";
    cout << "1. Add New Doctor\n";
    cout << "2. Add New Appointment\n";
    cout << "3. Update Doctor Name (Doctor ID)\n";
    cout << "4. Update Appointment Date (Appointment ID)\n";
    cout << "5. Delete Appointment (Appointment ID)\n";
    cout << "6. Delete Doctor (Doctor ID)\n";
    cout << "7. Print Doctor Info (Doctor ID)\n";
    cout << "8. Print Appointment Info (Appointment ID)\n";
    cout << "9. Write Query\n";
    cout << "10. Exit\n";
    cout << "Select an option: ";
}

int main() {
    FileManager fileManager;
    PrimaryIndex primaryIndex;
    SecondaryIndex secondaryIndex;
    Operations ops(fileManager, primaryIndex, secondaryIndex);

    fileManager.open("Doctors.dat", "Appointments.dat");
     primaryIndex.loadIndex("primary.idx");
     secondaryIndex.loadIndex("secondary.idx");
    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                 ops.handleAddNewDoctor();
                break;
            case 2:
                 ops.handleAddNewAppointment();
                break;
            case 3:
                 ops.handleUpdateDoctor();
                break;
            case 4:
                 ops.handleUpdateAppointmentDate();
                break;
            case 5:
                 ops.handleDeleteAppointment();
                break;
            case 6:
                 ops.handleDeleteDoctor();
                break;
            case 7:
                 ops.handlePrintDoctor();
                break;
            case 8:
                 ops.handlePrintAppointment();
                break;
            case 9:
                 ops.handleQuery();
                break;
            case 10:
                running = false;
                cout << "Exiting...\n";
                break;
            default:
                cout << "Unknown option. Please try again.\n";
                break;
        }
    }

    primaryIndex.saveIndex();
    secondaryIndex.saveIndex();
    fileManager.close();
    return 0;
}