#include <MFRC522.h>    // uncomment in Sketch
#include <SPI.h>

#define STATION_NUM 0 // station number 1-4 or 0 if check-in

#define SS_PIN 10
#define RST_PIN 9

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

  digitalWrite(8, HIGH); // LED on the output 8

  Serial.print("UUID:");
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.print(" " + String(nuidPICC[i]));
  }
  Serial.println(".");

  delay(500);
  digitalWrite(8, LOW); // LED off on output 8

}
