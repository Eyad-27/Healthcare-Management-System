#include <bits/stdc++.h>
using namespace std;

#include "Operations.h"

// TODO: Team Member 4 (CRUD Logic) to implement non-query handlers.

Operations::Operations(FileManager& fm, PrimaryIndex& pidx, SecondaryIndex& sidx)
    : fm_(fm), pidx_(pidx), sidx_(sidx) {}

void Operations::handleAddNewDoctor() {
    int id;
    string name, address;
    cout << "Enter Doctor ID: ";
    cin >> id;
    cout << "Enter Doctor Name: ";
    cin >> name;
    cout << "Enter Doctor Address: ";
    cin >> address;
    Doctor doc;
    copyToBuf(to_string(id), doc.DoctorID, sizeof(doc.DoctorID));
    copyToBuf(name, doc.DoctorName, sizeof(doc.DoctorName));
    copyToBuf(address, doc.Address, sizeof(doc.Address));
    string recordStr = string(doc.DoctorID) + "|" + string(doc.DoctorName) + "|" + string(doc.Address) + "$";
    pidx_.addEntry(doc.DoctorID, recordStr);
}

void Operations::handleAddNewAppointment() {
    int appId;
    string appDate;
    int docId;
    cout << "Enter Appointment ID: ";
    cin >> appId;
    cout << "Enter Appointment Date: ";
    cin >> appDate;
    cout << "Enter Doctor ID: ";
    cin >> docId;
    Appointment app;
    copyToBuf(to_string(appId), app.AppointmentID, sizeof(app.AppointmentID));
    copyToBuf(appDate, app.AppointmentDate, sizeof(app.AppointmentDate));
    copyToBuf(to_string(docId), app.DoctorID, sizeof(app.DoctorID));
    string recordStr = string(app.AppointmentID) + "|" + string(app.AppointmentDate) + "|" + string(app.DoctorID) + "$";
    pidx_.addEntry(app.AppointmentID, recordStr);
}

void Operations::handleDeleteDoctor() {
    string docId;
    cout << "Enter Doctor ID to delete: ";
    cin >> docId;
    int getOff = pidx_.search(docId);
    if (getOff < 0) {
        cout << "Doctor ID not found.\n";
        return;
    }
    Doctor doc = fm_.getDoctor(getOff);
    fm_.deleteDoctor(getOff);
    pidx_.deleteEntry(docId);
    sidx_.deleteEntry(doc.DoctorName, docId);

}

void Operations::handleDeleteAppointment() {
    string appId;
    cout << "Enter Appointment ID to delete: ";
    cin >> appId;
    int getOff = pidx_.search(appId);
    if (getOff < 0) {
        cout << "Appointment ID not found.\n";
        return;
    }
    Appointment app = fm_.getAppointment(getOff);
    fm_.deleteAppointment(getOff);
    pidx_.deleteEntry(appId);
    sidx_.deleteEntry(app.DoctorID, appId);
}

void Operations::handleUpdateDoctor() {
    int id;
    string newName;
    cout << "Enter Doctor ID to update: ";
    cin >> id;
    cout << "Enter new Doctor Name: ";
    cin >> newName;
    string docIdStr = to_string(id);
    int getOff = pidx_.search(docIdStr);
    if (getOff < 0) {
        cout << "Doctor ID not found.\n";
        return;
    }
    Doctor doc = fm_.getDoctor(getOff);
    copyToBuf(newName, doc.DoctorName, sizeof(doc.DoctorName));
    fm_.updateDoctor(getOff, doc);
}

void Operations::handleUpdateAppointmentDate() {
    int appId;
    string newDate;
    cout << "Enter Appointment ID to update: ";
    cin >> appId;
    cout << "Enter new Appointment Date: ";
    cin >> newDate;
    string appIdStr = to_string(appId);
    int getOff = pidx_.search(appIdStr);
    if (getOff < 0) {
        cout << "Appointment ID not found.\n";
        return;
    }
    Appointment app = fm_.getAppointment(getOff);
    copyToBuf(newDate, app.AppointmentDate, sizeof(app.AppointmentDate));
    fm_.updateAppointment(getOff, app);

}

void Operations::handlePrintDoctor() {
    int id;
    cout << "Enter Doctor ID to print: ";
    cin >> id;
    string docIdStr = to_string(id);
    int getOff = pidx_.search(docIdStr);
    if (getOff < 0) {
        cout << "Doctor ID not found.\n";
        return;
    }
    Doctor doc = fm_.getDoctor(getOff);
    cout << "DoctorID: " << doc.DoctorID << " | Name: " << doc.DoctorName << " | Address: " << doc.Address << "\n";
}

void Operations::handlePrintAppointment() {
    int appId;
    cout << "Enter Appointment ID to print: ";
    cin >> appId;
    string appIdStr = to_string(appId);
    int getOff = pidx_.search(appIdStr);
    if (getOff < 0) {
        cout << "Appointment ID not found.\n";
        return;
    }
    Appointment app = fm_.getAppointment(getOff);
    cout << "AppointmentID: " << app.AppointmentID << " | Date: " << app.AppointmentDate << " | DoctorID: " << app.DoctorID << "\n";
}

static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

static string upper(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return toupper(c); });
    return s;
}

void Operations::copyToBuf(const string& src, char* dst, size_t dstSize) {
    if (!dst || dstSize == 0) return;
    size_t n = min(dstSize - 1, src.size());
    memcpy(dst, src.data(), n);
    dst[n] = '\0';
}

// Grammar accepted (case-insensitive, flexible spaces):
//   SELECT <field|ALL> FROM <Doctors|Appointments> WHERE <Field Name>='value';
// Supported fields:
//   Doctors: Doctor ID, Doctor Name, Address
//   Appointments: Appointment ID, Appointment Date, Doctor ID
// Linking to stubs:
//   - WHERE by primary key (Doctor ID or Appointment ID) -> use PrimaryIndex::search(key) then FileManager::get...
//   - WHERE by secondary field (Doctor Name or Doctor ID inside Appointments, or Appointment Date) -> use SecondaryIndex stubs
//   - SELECT ALL prints all fields of the matched record(s); selecting a single field prints just that field.
void Operations::handleQuery() {
    cout << "Enter SQL-like query (e.g., Select all from Doctors where Doctor ID='D001';) or BACK to return\n> ";

    if (cin.peek() == '\n') cin.get();
    string line;
    if (!getline(cin, line)) return;
    line = trim(line);
    if (line.empty() || upper(line) == "BACK") return;

    // Normalize spacing and split into clauses: SELECT ... FROM ... WHERE ... ;
    // We'll find keywords case-insensitively.
    string u = upper(line);

    auto findKw = [&](const string& kw) { return u.find(kw); };

    size_t selPos = findKw("SELECT ");
    size_t fromPos = findKw(" FROM ");
    size_t wherePos = findKw(" WHERE ");

    if (selPos == string::npos || fromPos == string::npos || fromPos <= selPos) {
        cout << "Syntax error: expected 'Select ... from ...'\n";
        return;
    }

    string selectPart = trim(line.substr(selPos + 7, fromPos - (selPos + 7)));

    string tablePart;
    string wherePart;
    if (wherePos == string::npos) {
        tablePart = trim(line.substr(fromPos + 6));
    } else {
        tablePart = trim(line.substr(fromPos + 6, wherePos - (fromPos + 6)));
        wherePart = trim(line.substr(wherePos + 7));
    }

    // Remove trailing semicolon from where/table if present
    if (!wherePart.empty() && wherePart.back() == ';') wherePart.pop_back();
    if (!tablePart.empty() && tablePart.back() == ';') tablePart.pop_back();

    // Determine table
    string tableU = upper(tablePart);
    bool isDoctors = tableU == "DOCTORS";
    bool isAppointments = tableU == "APPOINTMENTS";
    if (!isDoctors && !isAppointments) {
        cout << "Unknown table: " << tablePart << " (expected Doctors or Appointments)\n";
        return;
    }

    // Parse WHERE: <Field Name>='value'
    string fieldName;
    string value;
    if (!wherePart.empty()) {
        auto eqPos = wherePart.find('=');
        if (eqPos == string::npos) {
            cout << "Syntax error in WHERE clause (expected = )\n";
            return;
        }
        fieldName = trim(wherePart.substr(0, eqPos));
        string rhs = trim(wherePart.substr(eqPos + 1));
        // Expect quoted value '...'
        if (rhs.size() >= 2 && rhs.front() == '\'' && rhs.back() == '\'') {
            value = rhs.substr(1, rhs.size() - 2);
        } else {
            // Allow without quotes as a fallback
            value = rhs;
        }
    }

    // Helper lambdas to print records
    auto printDoctor = [&](const Doctor& d, bool all, const string& selFieldU) {
        if (all) {
            cout << "DoctorID: " << d.DoctorID << " | Name: " << d.DoctorName << " | Address: " << d.Address << "\n";
            return;
        }
        if (selFieldU == "DOCTOR NAME") {
            cout << d.DoctorName << "\n";
        } else if (selFieldU == "DOCTOR ID") {
            cout << d.DoctorID << "\n";
        } else if (selFieldU == "ADDRESS") {
            cout << d.Address << "\n";
        } else {
            cout << "Unknown field in SELECT for Doctors: " << selFieldU << "\n";
        }
    };

    auto printAppointment = [&](const Appointment& a, bool all, const string& selFieldU) {
        if (all) {
            cout << "AppointmentID: " << a.AppointmentID << " | Date: " << a.AppointmentDate << " | DoctorID: " << a.DoctorID << "\n";
            return;
        }
        if (selFieldU == "APPOINTMENT ID") {
            cout << a.AppointmentID << "\n";
        } else if (selFieldU == "APPOINTMENT DATE") {
            cout << a.AppointmentDate << "\n";
        } else if (selFieldU == "DOCTOR ID") {
            cout << a.DoctorID << "\n";
        } else {
            cout << "Unknown field in SELECT for Appointments: " << selFieldU << "\n";
        }
    };

    // Interpret SELECT
    string selectU = upper(selectPart);
    bool selectAll = (selectU == "ALL");

    // Dispatch by where field and table
    if (isDoctors) {
        // WHERE possibilities: Doctor ID, Doctor Name, Address
        string fieldU = upper(fieldName);
        if (fieldU.empty()) {
            cout << "WHERE clause is required to limit results in this stub version.\n";
            return;
        }
        if (fieldU == "DOCTOR ID") {
            // primary index path: search primary by key, then fetch doctor
            int rrn = pidx_.search(value);
            if (rrn < 0) {
                cout << "No doctor with ID '" << value << "'\n";
                return;
            }
            Doctor d = fm_.getDoctor(rrn);
            printDoctor(d, selectAll, selectU);
        } else if (fieldU == "DOCTOR NAME" || fieldU == "ADDRESS") {
            // secondary index path (stub): search secondary -> list of primary keys
            vector<string> keys = sidx_.search(value);
            if (keys.empty()) {
                cout << "No matching doctors found.\n";
                return;
            }
            for (const auto& k : keys) {
                int rrn = pidx_.search(k); // map key to RRN via primary index (stub)
                if (rrn >= 0) {
                    Doctor d = fm_.getDoctor(rrn);
                    printDoctor(d, selectAll, selectU);
                }
            }
        } else {
            cout << "Unsupported WHERE field for Doctors: " << fieldName << "\n";
            return;
        }
    } else if (isAppointments) {
        // WHERE possibilities: Appointment ID, Appointment Date, Doctor ID
        string fieldU = upper(fieldName);
        if (fieldU.empty()) {
            cout << "WHERE clause is required to limit results in this stub version.\n";
            return;
        }
        if (fieldU == "APPOINTMENT ID") {
            int rrn = pidx_.search(value);
            if (rrn < 0) {
                cout << "No appointment with ID '" << value << "'\n";
                return;
            }
            Appointment a = fm_.getAppointment(rrn);
            printAppointment(a, selectAll, selectU);
        } else if (fieldU == "DOCTOR ID" || fieldU == "APPOINTMENT DATE") {
            // secondary index path: search secondary -> list of appointment primary keys
            vector<string> keys = sidx_.search(value);
            if (keys.empty()) {
                cout << "No matching appointments found.\n";
                return;
            }
            for (const auto& k : keys) {
                int rrn = pidx_.search(k);
                if (rrn >= 0) {
                    Appointment a = fm_.getAppointment(rrn);
                    printAppointment(a, selectAll, selectU);
                }
            }
        } else {
            cout << "Unsupported WHERE field for Appointments: " << fieldName << "\n";
            return;
        }
    }
}
