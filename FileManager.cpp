#include <bits/stdc++.h>
using namespace std;
#include "FileManager.h"


int FileManager::addDoctor(Doctor record) {
    if (!doctorsFile.is_open())
        return -1;

    doctorsFile.clear();

    // Build new record buffer
    char *newRecord = new char[79];
    strcpy(newRecord, record.DoctorID); strcat(newRecord, "|");
    strcat(newRecord, record.DoctorName); strcat(newRecord, "|");
    strcat(newRecord, record.Address); strcat(newRecord, "$");
    short newLength = (short)strlen(newRecord);

    // Try to reuse from avail list (first-fit)
    short header;
    doctorsFile.seekg(0, ios::beg);
    doctorsFile.read((char*)&header, sizeof(header));

    short chosenOffset = -1;
    short prevOffset = -1;
    short currOffset = header;

    while (currOffset != -1) {
        // Read node length and next pointer
        doctorsFile.seekg(currOffset, ios::beg);
        short slotLen; doctorsFile.read((char*)&slotLen, sizeof(slotLen));
        // Position after reading length
        // Read deletion marker and next pointer
        char delMark;
        doctorsFile.read(&delMark, 1);
        short nextOffset; doctorsFile.read((char*)&nextOffset, sizeof(nextOffset));

        if (slotLen >= newLength) {
            chosenOffset = currOffset;
            // Update header or link previous to next
            if (prevOffset == -1) {
                // Consuming head
                doctorsFile.seekp(0, ios::beg);
                doctorsFile.write((char*)&nextOffset, sizeof(nextOffset));
            } else {
                // Link previous node's next to nextOffset
                doctorsFile.seekg(prevOffset, ios::beg);
                short prevLen; doctorsFile.read((char*)&prevLen, sizeof(prevLen));
                // skip delMark, then overwrite next pointer of prev
                char tmp;
                doctorsFile.read(&tmp, 1);
                doctorsFile.seekp(doctorsFile.tellg(), ios::beg);
                doctorsFile.write((char*)&nextOffset, sizeof(nextOffset));
            }
            break;
        }
        prevOffset = currOffset;
        currOffset = nextOffset;
    }

    if (chosenOffset != -1) {
        // Write into chosen slot
        doctorsFile.seekp(chosenOffset, ios::beg);
        doctorsFile.write((char*)&newLength, sizeof(newLength));
        doctorsFile.write(newRecord, newLength);
        delete[] newRecord;
        return chosenOffset;
    }

    // Append at end
    doctorsFile.seekp(0, ios::end);
    short offset = (short)doctorsFile.tellp();
    doctorsFile.write((char*)&newLength, sizeof(newLength));
    doctorsFile.write(newRecord, newLength);
    delete[] newRecord;
    return offset;
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

    stringstream ss(record);
    Doctor doctor;
    ss.getline(doctor.DoctorID, 15, '|');
    ss.getline(doctor.DoctorName, 30, '|');
    ss.getline(doctor.Address, 30, '$');

    delete[] record;
    return doctor;
}

short FileManager::deleteDoctor(int offset) {

    doctorsFile.clear();
    // header of the Avail List
    short header;
    doctorsFile.seekg(0, ios::beg);
    doctorsFile.read((char*)&header, sizeof(header));

    doctorsFile.seekg(offset, ios::beg);

    // Length of the deleted record
    short length;
    doctorsFile.read((char*)&length, sizeof(length));

    doctorsFile.seekp(doctorsFile.tellg(), ios::beg);
    // flag that this record is deleted
    doctorsFile.write("*", 1);
    // pointer to the previous deleted record offset
    doctorsFile.write((char*)&header, sizeof(header));

    // seek to the beginning of the file to modify the avail list header
    doctorsFile.seekp(0, ios::beg);
    header = (short)offset;
    doctorsFile.write((char*)&header, sizeof(header));
    return header;

}

int FileManager::updateDoctor(int offset, const Doctor& record) {

    doctorsFile.clear();
    doctorsFile.seekp(offset, ios::beg);
    char *newRecord = new char[79];
    strcpy(newRecord,record.DoctorID); strcat(newRecord, "|");
    strcat(newRecord,record.DoctorName); strcat(newRecord, "|");
    strcat(newRecord,record.Address);strcat(newRecord,"$");
    short length = (short)strlen(newRecord);
    doctorsFile.write((char*)&length, sizeof(length));
    doctorsFile.write(newRecord, length);
    delete[] newRecord;
    return 0;
}

int FileManager::addAppointment(Appointment record) {
    if (!appointmentsFile.is_open())
        return -1;

    appointmentsFile.clear();

    char *newRecord = new char[64];
    strcpy(newRecord, record.AppointmentID); strcat(newRecord, "|");
    strcat(newRecord, record.AppointmentDate); strcat(newRecord, "|");
    strcat(newRecord, record.DoctorID); strcat(newRecord, "$");

    short newLength = (short)strlen(newRecord);

    // Try to reuse from avail list (first-fit)
    short header;
    appointmentsFile.seekg(0, ios::beg);
    appointmentsFile.read((char*)&header, sizeof(header));

    short chosenOffset = -1;
    short prevOffset = -1;
    short currOffset = header;

    while (currOffset != -1) {
        appointmentsFile.seekg(currOffset, ios::beg);
        short slotLen; appointmentsFile.read((char*)&slotLen, sizeof(slotLen));
        char delMark; appointmentsFile.read(&delMark, 1);
        short nextOffset; appointmentsFile.read((char*)&nextOffset, sizeof(nextOffset));

        if (slotLen >= newLength) {
            chosenOffset = currOffset;
            if (prevOffset == -1) {
                appointmentsFile.seekp(0, ios::beg);
                appointmentsFile.write((char*)&nextOffset, sizeof(nextOffset));
            } else {
                appointmentsFile.seekg(prevOffset, ios::beg);
                short prevLen; appointmentsFile.read((char*)&prevLen, sizeof(prevLen));
                char tmp; appointmentsFile.read(&tmp, 1);
                appointmentsFile.seekp(appointmentsFile.tellg(), ios::beg);
                appointmentsFile.write((char*)&nextOffset, sizeof(nextOffset));
            }
            break;
        }
        prevOffset = currOffset;
        currOffset = nextOffset;
    }

    if (chosenOffset != -1) {
        appointmentsFile.seekp(chosenOffset, ios::beg);
        appointmentsFile.write((char*)&newLength, sizeof(newLength));
        appointmentsFile.write(newRecord, newLength);
        delete[] newRecord;
        return chosenOffset;
    }

    appointmentsFile.seekp(0, ios::end);
    int offset = (int)appointmentsFile.tellp();
    appointmentsFile.write((char*)&newLength, sizeof(newLength));
    appointmentsFile.write(newRecord, newLength);
    delete[] newRecord;
    return offset;
}

Appointment FileManager::getAppointment(int offset) {
    appointmentsFile.clear();
    // move the read pointer to the offset for direct access
    appointmentsFile.seekg(offset, ios::beg);
    // to store the length of the record
    short length;
    appointmentsFile.read((char*)&length, sizeof(length));

    char *record = new char[length + 1];
    record[length] = '\0';
    appointmentsFile.read(record, length);

    stringstream ss(record);
    Appointment appointment;
    ss.getline(appointment.AppointmentID, 15, '|');
    ss.getline(appointment.AppointmentDate, 30, '|');
    ss.getline(appointment.DoctorID, 15, '$');

    delete[] record;
    return appointment;
}

short FileManager::deleteAppointment(int offset) {

    appointmentsFile.clear();
    short header;
    appointmentsFile.seekg(0, ios::beg);
    appointmentsFile.read((char*)&header, sizeof(header));

    appointmentsFile.seekg(offset, ios::beg);

    short length;
    appointmentsFile.read((char*)&length, sizeof(length));

    appointmentsFile.seekp(appointmentsFile.tellg(), ios::beg);
    appointmentsFile.write("*", 1);
    appointmentsFile.write((char*)&header, sizeof(header));

    appointmentsFile.seekp(0, ios::beg);
    header = (short)offset;
    appointmentsFile.write((char*)&header, sizeof(header));
    return header;
}

int FileManager::updateAppointment(int offset, const Appointment& record) {

    appointmentsFile.clear();
    appointmentsFile.seekp(offset, ios::beg);
    char *newRecord = new char[64];
    strcpy(newRecord,record.AppointmentID); strcat(newRecord, "|");
    strcat(newRecord,record.AppointmentDate); strcat(newRecord, "|");
    strcat(newRecord,record.DoctorID);strcat(newRecord,"$");
    short length = (short)strlen(newRecord);
    appointmentsFile.write((char*)&length, sizeof(length));
    appointmentsFile.write(newRecord, length);
    delete[] newRecord;
    return 0;
}

void FileManager::emptyDocAvailList() {
    doctorsFile.clear();
    doctorsFile.seekp(0, ios::beg);
    short header = -1;
    doctorsFile.write((char*)&header, sizeof(header));
}
void FileManager::emptyAppAvailList() {
    appointmentsFile.clear();
    appointmentsFile.seekp(0, ios::beg);
    short header = -1;
    appointmentsFile.write((char*)&header, sizeof(header));
}

void FileManager::open(const string& doctorsFile, const string& appointmentsFile) {
    // Open or create doctors file
    this->doctorsFile.open(doctorsFile, ios::in | ios::out | ios::binary);
    if (!this->doctorsFile.is_open()) {
        // Create then reopen
        fstream create(doctorsFile, ios::out | ios::binary);
        short header = -1;
        create.write((char*)&header, sizeof(header));
        create.close();
        this->doctorsFile.open(doctorsFile, ios::in | ios::out | ios::binary);
    } else {
        // If empty, write -1 header
        this->doctorsFile.seekg(0, ios::end);
        auto size = this->doctorsFile.tellg();
        if (size < (streampos)sizeof(short)) {
            this->doctorsFile.clear();
            this->doctorsFile.seekp(0, ios::beg);
            short header = -1;
            this->doctorsFile.write((char*)&header, sizeof(header));
            this->doctorsFile.flush();
        }
    }

    // Open or create appointments file
    this->appointmentsFile.open(appointmentsFile, ios::in | ios::out | ios::binary);
    if (!this->appointmentsFile.is_open()) {
        fstream create(appointmentsFile, ios::out | ios::binary);
        short header = -1;
        create.write((char*)&header, sizeof(header));
        create.close();
        this->appointmentsFile.open(appointmentsFile, ios::in | ios::out | ios::binary);
    } else {
        this->appointmentsFile.seekg(0, ios::end);
        auto size = this->appointmentsFile.tellg();
        if (size < (streampos)sizeof(short)) {
            this->appointmentsFile.clear();
            this->appointmentsFile.seekp(0, ios::beg);
            short header = -1;
            this->appointmentsFile.write((char*)&header, sizeof(header));
            this->appointmentsFile.flush();
        }
    }
}

void FileManager::close() {
    this->doctorsFile.close();
    this->appointmentsFile.close();
}
