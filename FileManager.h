#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "DataStructs.h"

// FileManager is responsible for storing and retrieving records from persistent storage.
class FileManager {
private:
    fstream doctorsFile;
    fstream appointmentsFile;
public:
    // Doctor operations
    int addDoctor(Doctor record);
    Doctor getDoctor(int offset);
    void deleteDoctor(int offset);
    int updateDoctor(int offset, const Doctor& record);

    // Appointment operations
    int addAppointment(Appointment record);
    Appointment getAppointment(int offset);
    void deleteAppointment(int offset);
    int updateAppointment(int offset, const Appointment& record);

    // Persist and housekeeping
    void open(const string& doctorsFile, const string& appointmentsFile);
    void close();
};
