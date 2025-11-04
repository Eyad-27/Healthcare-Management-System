#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "DataStructs.h"

// FileManager is responsible for storing and retrieving records from persistent storage.
class FileManager {
public:
    // Doctor operations
    int addDoctor(Doctor record);
    Doctor getDoctor(int rrn);
    void deleteDoctor(int rrn);
    int updateDoctor(int rrn, const Doctor& record);

    // Appointment operations
    int addAppointment(Appointment record);
    Appointment getAppointment(int rrn);
    void deleteAppointment(int rrn);
    int updateAppointment(int rrn, const Appointment& record);

    // Persist and housekeeping
    void open(const string& doctorsFile, const string& appointmentsFile);
    void close();
};
