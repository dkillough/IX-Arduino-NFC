#include <MFRC522.h>    // uncomment in Sketch
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 5

#define LED_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

byte nuidPICC[4];
byte block = 1;

MFRC522::StatusCode status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(LED_PIN, OUTPUT); // initialization of LED_PIN as "Output"
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reset the loop if no new card present on the sensor/reader.
  if ( ! rfid.PICC_IsNewCardPresent()) return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial()) return;

  digitalWrite(LED_PIN, HIGH); // LED on the output 8
  
  auth();
  printUUID();

  checkCard();
  
  delay(400);
  digitalWrite(LED_PIN, LOW); // LED off on output 8

}

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
    // Serial.println(F("PCD_Authenticate(): SUCCESS"));
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

void checkCard() {
  byte buffer[18];
  byte buffer_size = sizeof(buffer);

  status = rfid.MIFARE_Read(block, buffer, &buffer_size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("INITIAL READ FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    // Serial.println(F("INITIAL READ SUCCESS"));
    // Serial.print("Card data: ");
    // Serial.print(buffer[0]);
    // Serial.println(". Should read some value between 0 and 255 inclusive.");
  }

  byte bufferSet[18] = {buffer[0]};
  byte data = buffer[0];
  buffer_size = 18;

  // if they visited station 1
  if(int(data & 0b00001000) == 8) {
    bufferSet[0] |= 0b10000000;
  }

  // if they visited station 2
  if(int(data & 0b00000100) == 4) {
    Serial.println("aaa");
    bufferSet[0] |= 0b01000000;
  }

  // if they visited station 3
  if(int(data & 0b000000010) == 2) {
    bufferSet[0] |= 0b00100000;
  }

  // if they visited station 4
  if(int(data & 0b000000001) == 1) {
    bufferSet[0] |= 0b00010000;
  }

  status = rfid.MIFARE_Write(block, bufferSet, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("WRITE FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    // Serial.println(F("WRITE SUCCESS"));
  }
  
  // Readback  
  status = rfid.MIFARE_Read(block, bufferSet, &buffer_size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("RE-READ FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    // Serial.println(F("RE-READ SUCCESS"));
    parseByte(bufferSet[0]);
    // Serial.print("Card data: ");
    // Serial.print(bufferSet[0]);
    // Serial.println(". Should read some value based on what's been checked in.");
  }
}

void parseByte(byte cardData) {
  Serial.println("Stations checked in: ");
  if(cardData & 0b10000000) { Serial.print("1 "); } 
  if(cardData & 0b01000000) { Serial.print("2 "); }
  if(cardData & 0b00100000) { Serial.print("3 "); }
  if(cardData & 0b00010000) { Serial.print("4");  }
  Serial.println("");
}