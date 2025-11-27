#include <bits/stdc++.h>
using namespace std;
#include "FileManager.h"


int FileManager::addDoctor(Doctor record) {
    // if the file didn't open successfully retrun
    if (!doctorsFile.is_open())
        return -1;

    doctorsFile.clear();
    // move the write pointer to the end of file to append the record
    doctorsFile.seekp(0, ios::end);

    // 15 (id) + 30 (name) + 30 (address)+ 1 (|)+ 1 (|)+ 1 ($) + 1(\0)  = 79 byte
    char *newRecord = new char[79];

    strcpy(newRecord, record.DoctorID); strcat(newRecord, "|");
    strcat(newRecord, record.DoctorName); strcat(newRecord, "|");
    strcat(newRecord, record.Address); strcat(newRecord, "$");

    short length = strlen(newRecord); // Length indicator for the record

    doctorsFile.write((char*)&length, sizeof(length));

    doctorsFile.write(newRecord, length);
    int getPos = doctorsFile.tellp();
    delete[] newRecord;
    // return RRN
    return getPos / (sizeof(length) + length);
}
//  Offset = Current Offset + Size of Length Indicator (2)+ Record Length
Doctor FileManager::getDoctor(int offset) {

    doctorsFile.clear();
    // move the read pointer to the offset for direct access
    doctorsFile.seekg(offset, ios::beg);
    // to store the length of the record
    short length;
    doctorsFile.read((char*)&length, sizeof(length));

    // to store the record itself (The Doctor)
    char *record = new char[length + 1];
    record[length] = '\0'; // to know the end of the c string
    doctorsFile.read(record, length);

    // make stream to separate the record array with delimiters
    stringstream ss(record);
    Doctor doctor;
    ss.getline(doctor.DoctorID, 15, '|');
    ss.getline(doctor.DoctorName, 30, '|');
    ss.getline(doctor.Address, 30, '$');

    delete[] record;
    return doctor;
}

void FileManager::deleteDoctor(int offset) {
    // TODO: Team Member 1 (File Manager) to implement this.
}

int FileManager::updateDoctor(int offset, const Doctor& record) {
    // TODO: Team Member 1 (File Manager) to implement this.
    return 0;
}

int FileManager::addAppointment(Appointment record) {
    if (!appointmentsFile.is_open())
        return -1;

    appointmentsFile.clear();
    // move the write pointer to the end of file to append the record
    appointmentsFile.seekp(0, ios::end);

    // 15 (id) + 30 (date) + 15 (DoctorID)+ 1 (|)+ 1 (|)+ 1 ($) + 1(\0)  = 64 byte
    char *newRecord = new char[64];

    strcpy(newRecord, record.AppointmentID); strcat(newRecord, "|");
    strcat(newRecord, record.AppointmentDate); strcat(newRecord, "|");
    strcat(newRecord, record.DoctorID); strcat(newRecord, "$");

    short length = strlen(newRecord); // Length indicator for the record

    appointmentsFile.write((char*)&length, sizeof(length));

    appointmentsFile.write(newRecord, length);
    int getPos = appointmentsFile.tellp();
     // return RRN
     int rrn = getPos / (sizeof(length) + length);

    delete[] newRecord;
    return rrn;
}

Appointment FileManager::getAppointment(int offset) {
    appointmentsFile.clear();
    // move the read pointer to the offset for direct access
    appointmentsFile.seekg(offset, ios::beg);
    // to store the length of the record
    short length;
    appointmentsFile.read((char*)&length, sizeof(length));

    // to store the record itself (The Appointment)
    char *record = new char[length + 1];
    record[length] = '\0'; // to know the end of the c string
    appointmentsFile.read(record, length);

    // make stream to separate the record array with delimiters
    stringstream ss(record);
    Appointment appointment;
    ss.getline(appointment.AppointmentID, 15, '|');
    ss.getline(appointment.AppointmentDate, 30, '|');
    ss.getline(appointment.DoctorID, 15, '$');

    delete[] record;
    return appointment;
}

void FileManager::deleteAppointment(int offset) {
    // TODO: Team Member 1 (File Manager) to implement this.
}

int FileManager::updateAppointment(int offset, const Appointment& record) {
    // TODO: Team Member 1 (File Manager) to implement this.
    return 0;
}

void FileManager::open(const string& doctorsFile, const string& appointmentsFile) {
    this->doctorsFile.open(doctorsFile,ios::in | ios::out | ios::binary);
    this->appointmentsFile.open(appointmentsFile,ios::in | ios::out | ios::binary);
}

void FileManager::close() {
    this->doctorsFile.close();
    this->appointmentsFile.close();
}
