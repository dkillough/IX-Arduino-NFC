#include "checkin.h"

void scanData(MFRC522 rfid) {
    objData scannedData;
    scannedData.visits = rfid.visitByte;
    scannedData.times = rfid.times;
    userData[rfid.uuid] = scannedData;
}
