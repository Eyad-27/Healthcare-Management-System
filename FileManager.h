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
    short deleteDoctor(int offset);
    int updateDoctor(int offset, const Doctor& record);
    void emptyDocAvailList();
    // Appointment operations
    int addAppointment(Appointment record);
    Appointment getAppointment(int offset);
    short deleteAppointment(int offset);
    int updateAppointment(int offset, const Appointment& record);
    void emptyAppAvailList();

    // Persist and housekeeping
    void open(const string& doctorsFile, const string& appointmentsFile);
    void close();
};
