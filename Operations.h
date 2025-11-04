#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "DataStructs.h"
#include "FileManager.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"

// High-level operations that orchestrate user interactions and call lower layers
class Operations {
public:
    // Construct with system components (stubs will be called for now)
    Operations(FileManager& fm, PrimaryIndex& pidx, SecondaryIndex& sidx);

    void handleAddNewDoctor();
    void handleAddNewAppointment();

    void handleDeleteDoctor();
    void handleDeleteAppointment();

    void handleUpdateDoctor(); // Update Doctor Name (Doctor ID)
    void handleUpdateAppointmentDate(); // Update Appointment Date (Appointment ID)

    void handlePrintDoctor(); // Print Doctor Info (Doctor ID)
    void handlePrintAppointment(); // Print Appointment Info (Appointment ID)

    void handleQuery(); // Handle SQL-like query input

private:
    FileManager& fm_;
    PrimaryIndex& pidx_;
    SecondaryIndex& sidx_;

    static void copyToBuf(const string& src, char* dst, size_t dstSize);
};
