#include "stations.h"
//
//void setWhichStationIAm(unsigned int num) {
//    Serial.println("stationNum called with " + num);
//    assert(num < 5);
//    stationNum = stationNumMap[num];
//    Serial.println("stationNum written as " + stationNum);
//}
//
//byte rwData(MFRC522 rfid) {
//    return rfid.visitByte = rfid.visitByte | stationNum;
//}
//
//time_t* calcTimes(MFRC522 rfid) {
//    time_t* ret = new time_t[4]{rfid.time[0], rfid.time[1], rfid.time[2], rfid.time[3]};
//    return ret;
//}
