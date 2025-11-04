#pragma once
#include <bits/stdc++.h>
using namespace std;

// Basic record structures used across the system
struct Doctor {
    char DoctorID[15];
    char DoctorName[30];
    char Address[30];
};

struct Appointment {
    char AppointmentID[15];
    char AppointmentDate[30];
    char DoctorID[15];
};
