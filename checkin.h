#ifndef CHECKIN_H
#define CHECKIN_H

#include <time.h>
#include <array>
#include <map>

// the idea: checkin and stations should have no knowledge of one another

struct objData
{
    char visits;
    std::array<time_t, 4> times;
};

std::map<char*, objData> userData;    // container data structure for all entries to this computer while program running

void scanData(MFRC522 rfid);

void exportDataToSheets();            // need some way of cloud storing the information

#endif