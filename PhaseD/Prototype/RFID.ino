#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 3

const int PIN_RED   = 5;
const int PIN_GREEN = 6;
const int PIN_BLUE  = 9;

MFRC522 rfid(SS_PIN, RST_PIN);

byte keyTagUID[4] = {0x87, 0x8B, 0xBD, 0xB4};

void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  analogWrite(PIN_RED,   100);
  analogWrite(PIN_GREEN, 100);
  analogWrite(PIN_BLUE,  100);

  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {
  
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      
      if (rfid.uid.uidByte[0] == keyTagUID[0] &&
          rfid.uid.uidByte[1] == keyTagUID[1] &&
          rfid.uid.uidByte[2] == keyTagUID[2] &&
          rfid.uid.uidByte[3] == keyTagUID[3] ) {
        Serial.println("Access is granted");
         analogWrite(PIN_RED,   0);
         analogWrite(PIN_GREEN, 255);
         analogWrite(PIN_BLUE,  0);
         delay(2000);
         rainbowLed();


      }
      else
      {
        Serial.print("Access denied for user with UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
         analogWrite(PIN_RED,   255);
         analogWrite(PIN_GREEN, 0);
         analogWrite(PIN_BLUE,  0);
      }
    }
  }
  
}

void rainbowLed(){
  int duration = 2500;  // Total duration for each color transition (in milliseconds)
  int colorSpeed = 50;   // Speed of the color transition (adjust as desired)

  unsigned long startTime = millis();

  while (millis() - startTime < duration) {
    float progress = (float)(millis() - startTime) / duration;

    // Calculate the color intensities based on the progress
    int redIntensity = (int)(255 * (1 - progress));
    int greenIntensity = 0;
    int blueIntensity = (int)(255 * progress);

    // Set the intensities for each color component using analogWrite()
    analogWrite(PIN_RED, redIntensity);
    analogWrite(PIN_GREEN, greenIntensity);
    analogWrite(PIN_BLUE, blueIntensity);

    // Delay for a short period to control the speed of the color transition
    delay(colorSpeed);
  }
}

