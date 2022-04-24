#ifndef CHECKIN_H
#define CHECKIN_H

#include <time.h>

// the idea: checkin and stations should have no knowledge of one another

struct objData
{
    char* uid;
    char visits;
    time_t times[4];
};

// objData userData[20];    // container data structure for all entries to this computer while program running

// void scanData(MFRC522 rfid);

// void exportDataToSheets();            // need some way of cloud storing the information

#endif
