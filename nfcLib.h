#ifndef NFCLIB_H
#define NFCLIB_H

#include <MFRC522.h>    // uncomment in Sketch
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::StatusCode status;
byte block = 1;
byte nuidPICC[4];

void auth() {
  MFRC522::MIFARE_Key key;
  key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  // AUTHENTICATE USING KEY A
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    Serial.println(F("PCD_Authenticate(): SUCCESS"));
  }
}

void printUUID() {
  Serial.print("UUID:");
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.print(" " + String(nuidPICC[i]));
  }
  Serial.println(".");
}

#endif