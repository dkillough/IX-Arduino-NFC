#include <MFRC522.h>    // uncomment in Sketch
#include <SPI.h>

#define STATION_NUM 0 // station number 1-4 or 0 if check-in

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
  pinMode(LED_PIN, OUTPUT); // initialization of LED_PIN int definition as "Output"
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reset the loop if no new card present on the sensor/reader.
  if ( ! rfid.PICC_IsNewCardPresent()) return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial()) return;

  digitalWrite(LED_PIN, HIGH); // turn on LED

  auth();

  printUUID();
  initCard();

  delay(400); // sleep for 200ms so the LED actually stays on for some amount of time
  digitalWrite(LED_PIN, LOW); // turn off LED
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

void initCard() {
  // check NFC read/write before handing them passport
  byte bufferSet[] = {0};
  status = rfid.MIFARE_Write(block, bufferSet, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("MIFARE_Write(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    // Serial.println(F("MIFARE_Write(): SUCCESS"));
  }

  // check card
  byte buffer[18];
  byte buffer_size = sizeof(buffer);

  status = rfid.MIFARE_Read(block, buffer, &buffer_size);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(F("MIFARE_Read(): FAILED"));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  } else {
    // Serial.println(F("MIFARE_Read(): SUCCESS"));
    Serial.print("Card data: ");
    Serial.print(buffer[0]);
    Serial.println(". Should read 0.");
  }
}