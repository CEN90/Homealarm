#include "controller.hpp"


void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD
}

void loop() {
    readCard();
    // writeCard();

    byte key = kpd.getKey();
    if(key) {
        Serial.println(key);
    }
}

void readCard() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte buffer[20];
    byte buffer_len = 20;

    if (readCardData(buffer, buffer_len))
    {
        Serial.println((char*) buffer);

        if (strcmp((char *) buffer, (char *) valid_name) == 0) {
            valid_key = true;
            Serial.println("Valid key found");
        } else {
            valid_key = false;
            Serial.println("Invalid key found");
        }
    }

    mfrc522.PCD_Reset();
}



