#include <MFRC522.h>    // uncomment in Sketch
#include <SPI.h>

#define STATION_NUM 0 // station number 1-4 or 0 if check-in

#define SS_PIN 10
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

byte nuidPICC[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(8, OUTPUT); // initialization of output 8 as "Output"
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reset the loop if no new card present on the sensor/reader.
  if ( ! rfid.PICC_IsNewCardPresent()) return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) return;

  byte block = 1;
  byte buffer[18];
  byte buffer_size = sizeof(buffer);
  MFRC522::MIFARE_Key key;
  MFRC522::StatusCode status;
  key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    // AUTHENTICATE USING KEY A : STARTS
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    Serial.println(F("PCD_Authenticate(): SUCCESS"));
  }
  // AUTHENTICATE USING KEY A : ENDS

  digitalWrite(8, HIGH); // LED on the output 8

  Serial.print("UUID:");
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.print(" " + String(nuidPICC[i]));
  }
  Serial.println(".");

  byte data_set[] = { 7, 16, 12, 34, 100, 0x5, 0x24, 0327, 0x28, 0x019, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f1 };

  status = rfid.MIFARE_Write(block, data_set, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("MIFARE_Write(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    Serial.println(F("MIFARE_Write(): SUCCESS"));
  }

  status = rfid.MIFARE_Read(block, buffer, &buffer_size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("MIFARE_Read(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    Serial.println(F("MIFARE_Read(): SUCCESS"));
    Serial.println("Data returned by rfid:");
    for(byte eb = 0; eb < buffer_size; eb++) {
      Serial.println(buffer[eb]);
    }
  }

  delay(500);
  digitalWrite(8, LOW); // LED off on output 8

}