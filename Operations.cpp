#include <bits/stdc++.h>
using namespace std;

#include "Operations.h"


Operations::Operations(FileManager& fm, PrimaryIndex& drsPrim, PrimaryIndex& aptsPrim, SecondaryIndex& drsSec, SecondaryIndex& aptsSec)
    : fm_(fm), drsPidx_(drsPrim), aptsPidx_(aptsPrim), drsSidx_(drsSec), aptsSidx_(aptsSec) {}

void Operations::handleAddNewDoctor() {
    int id;
    string name, address;
    cout << "Enter Doctor ID: ";
    cin >> id;
    cout << "Enter Doctor Name: ";
    cin >> name;
    cout << "Enter Doctor Address: ";
    cin >> address;
    Doctor doc{};
    copyToBuf(to_string(id), doc.DoctorID, sizeof(doc.DoctorID));
    copyToBuf(name, doc.DoctorName, sizeof(doc.DoctorName));
    copyToBuf(address, doc.Address, sizeof(doc.Address));
    long offset = fm_.addDoctor(doc);
    if (drsPidx_.addEntry(doc.DoctorID, offset)) {
        // Secondary keys for doctors (name, address -> DoctorID)
        drsSidx_.addEntry(doc.DoctorName, doc.DoctorID);
        drsSidx_.addEntry(doc.Address, doc.DoctorID);
        cout << "[OK] Doctor added. ID=" << doc.DoctorID << " Offset=" << offset << "\n";
        drsPidx_.saveIndex();
        drsSidx_.saveIndex();
    } else {
        cout << "[FAIL] Doctor ID already exists.\n";
    }
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
    Appointment app{};
    copyToBuf(to_string(appId), app.AppointmentID, sizeof(app.AppointmentID));
    copyToBuf(appDate, app.AppointmentDate, sizeof(app.AppointmentDate));
    copyToBuf(to_string(docId), app.DoctorID, sizeof(app.DoctorID));
    long offset = fm_.addAppointment(app);
    if (aptsPidx_.addEntry(app.AppointmentID, offset)) {
        // Secondary keys for appointments (DoctorID, Date -> AppointmentID)
        aptsSidx_.addEntry(app.DoctorID, app.AppointmentID);
        aptsSidx_.addEntry(app.AppointmentDate, app.AppointmentID);
        cout << "[OK] Appointment added. ID=" << app.AppointmentID << " Offset=" << offset << "\n";
        aptsPidx_.saveIndex();
        aptsSidx_.saveIndex();
    } else {
        cout << "[FAIL] Appointment ID already exists.\n";
    }
}

void Operations::handleDeleteDoctor() {
    string docId;
    cout << "Enter Doctor ID to delete: ";
    cin >> docId;
    long off = drsPidx_.search(docId);
    if (off < 0) {
        cout << "[FAIL] Doctor ID not found.\n";
        return;
    }
    Doctor doc = fm_.getDoctor((int)off);
    fm_.deleteDoctor((int)off);
    drsPidx_.deleteEntry(docId);
    drsSidx_.deleteEntry(doc.DoctorName, docId);
    drsSidx_.deleteEntry(doc.Address, docId);
    cout << "[OK] Doctor deleted. ID=" << docId << "\n";
    drsPidx_.saveIndex();
    drsSidx_.saveIndex();
}

void Operations::handleDeleteAppointment() {
    string appId;
    cout << "Enter Appointment ID to delete: ";
    cin >> appId;
    long off = aptsPidx_.search(appId);
    if (off < 0) {
        cout << "[FAIL] Appointment ID not found.\n";
        return;
    }
    Appointment app = fm_.getAppointment((int)off);
    fm_.deleteAppointment((int)off);
    aptsPidx_.deleteEntry(appId);
    aptsSidx_.deleteEntry(app.DoctorID, appId);
    aptsSidx_.deleteEntry(app.AppointmentDate, appId);
    cout << "[OK] Appointment deleted. ID=" << appId << "\n";
    aptsPidx_.saveIndex();
    aptsSidx_.saveIndex();
}

void Operations::handleUpdateDoctor() {
    int id;
    string newName;
    cout << "Enter Doctor ID to update: ";
    cin >> id;
    cout << "Enter new Doctor Name: ";
    cin >> newName;
    string key = to_string(id);
    long off = drsPidx_.search(key);
    if (off < 0) { cout << "[FAIL] Doctor ID not found.\n"; return; }
    Doctor doc = fm_.getDoctor((int)off);
    // Update secondary index if name changed
    if (string(doc.DoctorName) != newName) {
        drsSidx_.deleteEntry(doc.DoctorName, key);
        copyToBuf(newName, doc.DoctorName, sizeof(doc.DoctorName));
        drsSidx_.addEntry(doc.DoctorName, key);
    }
    fm_.updateDoctor((int)off, doc);
    cout << "[OK] Doctor name updated. ID=" << key << "\n";
    drsSidx_.saveIndex();
}

void Operations::handleUpdateAppointmentDate() {
    int appId;
    string newDate;
    cout << "Enter Appointment ID to update: ";
    cin >> appId;
    cout << "Enter new Appointment Date: ";
    cin >> newDate;
    string key = to_string(appId);
    long off = aptsPidx_.search(key);
    if (off < 0) { cout << "[FAIL] Appointment ID not found.\n"; return; }
    Appointment app = fm_.getAppointment((int)off);
    if (string(app.AppointmentDate) != newDate) {
        aptsSidx_.deleteEntry(app.AppointmentDate, key);
        copyToBuf(newDate, app.AppointmentDate, sizeof(app.AppointmentDate));
        aptsSidx_.addEntry(app.AppointmentDate, key);
    }
    fm_.updateAppointment((int)off, app);
    cout << "[OK] Appointment date updated. ID=" << key << "\n";
    aptsSidx_.saveIndex();
}

void Operations::handlePrintDoctor() {
    int id;
    cout << "Enter Doctor ID to print: ";
    cin >> id;
    string key = to_string(id);
    long off = drsPidx_.search(key);
    if (off < 0) { cout << "Doctor ID not found.\n"; return; }
    Doctor doc = fm_.getDoctor((int)off);
    cout << "DoctorID: " << doc.DoctorID << " | Name: " << doc.DoctorName << " | Address: " << doc.Address << "\n";
}

void Operations::handlePrintAppointment() {
    int appId;
    cout << "Enter Appointment ID to print: ";
    cin >> appId;
    string key = to_string(appId);
    long off = aptsPidx_.search(key);
    if (off < 0) { cout << "Appointment ID not found.\n"; return; }
    Appointment app = fm_.getAppointment((int)off);
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
//   - WHERE by secondary field (Doctor Name or Doctor ID inside Appointments, or Appointment Date) -> use SecondaryIndex
//   - SELECT ALL prints all fields of the matched record(s); selecting a single field prints just that field.
void Operations::handleQuery() {
    cout << "Enter SQL-like query (e.g., Select all from Doctors where Doctor ID='123';) or BACK to return\n> ";

    // Ensure we can read a full line even after numeric menu input
    if (cin.peek() == '\n') cin.get();
    string line;
    if (!getline(cin, line)) return;
    line = trim(line);
    if (line.empty() || upper(line) == "BACK") return;

    // Case-insensitive search helpers
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

    // Remove trailing semicolon
    if (!wherePart.empty() && wherePart.back() == ';') wherePart.pop_back();
    if (!tablePart.empty() && tablePart.back() == ';') tablePart.pop_back();

    // Validate table
    string tableU = upper(tablePart);
    bool isDoctors = (tableU == "DOCTORS");
    bool isAppointments = (tableU == "APPOINTMENTS");
    if (!isDoctors && !isAppointments) {
        cout << "Unknown table: " << tablePart << " (expected Doctors or Appointments)\n";
        return;
    }

    // Parse WHERE: <Field Name>='value' (quotes optional)
    string fieldName;
    string value;
    if (!wherePart.empty()) {
        // Normalize spaces around '=' to avoid parsing issues
        // e.g., "Doctor ID =  1" -> "Doctor ID=1"
        string normalizedWhere;
        normalizedWhere.reserve(wherePart.size());
        bool seenEq = false;
        for (size_t i = 0; i < wherePart.size(); ++i) {
            char c = wherePart[i];
            if (!seenEq && c == '=') {
                normalizedWhere.push_back('=');
                seenEq = true;
            } else if (!seenEq && isspace(static_cast<unsigned char>(c))) {
                // skip extra spaces before '='
            } else {
                normalizedWhere.push_back(c);
            }
        }
        auto eqPos = normalizedWhere.find('=');
        if (eqPos == string::npos) {
            cout << "Syntax error in WHERE clause (expected =)\n";
            return;
        }
        fieldName = trim(normalizedWhere.substr(0, eqPos));
        string rhs = trim(normalizedWhere.substr(eqPos + 1));
        if (!rhs.empty() && rhs.front() == '\'' && rhs.back() == '\'' && rhs.size() >= 2) {
            value = rhs.substr(1, rhs.size() - 2);
        } else {
            value = trim(rhs);
        }
    } else {
        cout << "Syntax error: missing WHERE clause\n";
        return;
    }

    // SELECT interpretation
    string selectU = upper(selectPart);
    bool selectAll = (selectU == "ALL");

    auto printDoctor = [&](const Doctor& d) {
        if (selectAll) {
            cout << "DoctorID: " << d.DoctorID << " | Name: " << d.DoctorName << " | Address: " << d.Address << "\n";
        } else if (selectU == "DOCTOR NAME") {
            cout << d.DoctorName << "\n";
        } else if (selectU == "DOCTOR ID") {
            cout << d.DoctorID << "\n";
        } else if (selectU == "ADDRESS") {
            cout << d.Address << "\n";
        } else {
            cout << "Unknown field in SELECT for Doctors: " << selectPart << "\n";
        }
    };

    auto printAppointment = [&](const Appointment& a) {
        if (selectAll) {
            cout << "AppointmentID: " << a.AppointmentID << " | Date: " << a.AppointmentDate << " | DoctorID: " << a.DoctorID << "\n";
        } else if (selectU == "APPOINTMENT ID") {
            cout << a.AppointmentID << "\n";
        } else if (selectU == "APPOINTMENT DATE") {
            cout << a.AppointmentDate << "\n";
        } else if (selectU == "DOCTOR ID") {
            cout << a.DoctorID << "\n";
        } else {
            cout << "Unknown field in SELECT for Appointments: " << selectPart << "\n";
        }
    };

    // Dispatch by table and WHERE field
    auto normalizeField = [](string f) {
        f = upper(trim(f));
        string out; out.reserve(f.size());
        for (char c : f) {
            if (!isspace(static_cast<unsigned char>(c))) out.push_back(c);
        }
        return out;
    };

    string fieldN = normalizeField(fieldName);
    size_t printed = 0;

    if (isDoctors) {
        if (fieldN == "DOCTORID") {
            // Primary key lookup via PrimaryIndex
            long offset = drsPidx_.search(value);
            if (offset < 0) {
                cout << "No doctor with ID '" << value << "'\n";
            } else {
                Doctor d = fm_.getDoctor((int)offset);
                printDoctor(d);
                printed++;
            }
        } else if (fieldN == "DOCTORNAME" || fieldN == "ADDRESS") {
            // Secondary key lookup via SecondaryIndex -> returns list of primary keys
            vector<string> keys = drsSidx_.search(value);
            if (keys.empty()) {
                cout << "No matching doctors found.\n";
            } else {
                for (const string& k : keys) {
                    long offset = drsPidx_.search(k);
                    if (offset >= 0) {
                        Doctor d = fm_.getDoctor((int)offset);
                        printDoctor(d);
                        printed++;
                    }
                }
                if (printed == 0) cout << "No matching doctors found in primary index.\n";
            }
        } else {
            cout << "Unsupported WHERE field for Doctors: " << fieldName << "\n";
        }
    } else if (isAppointments) {
        if (fieldN == "APPOINTMENTID") {
            long offset = aptsPidx_.search(value);
            if (offset < 0) {
                cout << "No appointment with ID '" << value << "'\n";
            } else {
                Appointment a = fm_.getAppointment((int)offset);
                printAppointment(a);
                printed++;
            }
        } else if (fieldN == "DOCTORID" || fieldN == "APPOINTMENTDATE") {
            vector<string> keys = aptsSidx_.search(value);
            if (keys.empty()) {
                cout << "No matching appointments found.\n";
            } else {
                for (const string& k : keys) {
                    long offset = aptsPidx_.search(k);
                    if (offset >= 0) {
                        Appointment a = fm_.getAppointment((int)offset);
                        printAppointment(a);
                        printed++;
                    }
                }
                if (printed == 0) cout << "No matching appointments found in primary index.\n";
            }
        } else {
            cout << "Unsupported WHERE field for Appointments: " << fieldName << "\n";
        }
    }
}
