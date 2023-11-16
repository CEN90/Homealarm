#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

// RFID reader relevant part
constexpr uint8_t RST_PIN = 9; 
constexpr uint8_t SS_PIN = 10; 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Keypad matrix
const byte ROWS = 2; // Four rows
const byte COLS = 2; // Three columns
byte keys[ROWS][COLS] = { {1, 3}, {2, 4} };
byte rowPins[ROWS] = { 2, 3 }; // 2, 3
byte colPins[COLS] = { 4, 5 }; // 4, 5
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Shit for Auth
size_t name_len = 11;
boolean valid_key = false;
byte valid_name[12] = {'C','a','r','l',' ','B','a','t','m','a','n','\0'};

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

boolean readCardData(byte *name, byte len) {
    byte block = 1;
   
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

    for (byte i = 0; i < 6; i++)
        key.keyByte[i] = 0xFF;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        
        return false;
    } 

    status = mfrc522.MIFARE_Read(block, name, &len);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        
        return false;
    }

    return true;
}

void writeCard() {
    byte name[16] = {'C','a','r','l',' ','B','a','t','m','a','n','\0',' ',' ',' ',' '};
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

    for (byte i = 0; i < 6; i++)
        key.keyByte[i] = 0xFF;

    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte block = 1;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    } else 
        Serial.println(F("PCD_Authenticate() success: ")); 

    status = mfrc522.MIFARE_Write(block, name, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    } else
        Serial.println(F("MIFARE_Write() success: "));
}
