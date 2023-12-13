#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

enum attacks_t {
    block_alarm,
    block_door_sensor,
    block_door_lock,
    block_auth,
    block_armed,
    none
};

constexpr uint8_t ATTACKS_LEN = 5;

// Description for attacks
String attack_labels[ATTACKS_LEN] = {
    "Blocking alarm signal.",
    "Blocking door sensor signal.",
    "Blocking door lock activation signal.",
    "Deactivating all authentication.",
    "Blocking armed signal, disabling alarm.",
};

attacks_t current_attack = none;

// Times used in the sketch
constexpr uint8_t POLL_TIME = 10; 
constexpr long KEYPRESS_TIME = 10000; 
constexpr long ERROR_KEY_TIME = 3000; 
constexpr int DOOR_LOCK_TIME = 500;

// Alarma!
constexpr uint8_t ALARM_OUTPUT = PIN4;
constexpr uint8_t ARMED_OUTPUT = PIN3;
// RFID reader relevant ports
constexpr uint8_t RST_PIN = 9; 
constexpr uint8_t SS_PIN = 10; 
// Door relevant ports
constexpr uint8_t DOOR_LOCK = PIN2;
constexpr uint8_t DOOR_SENSOR = PIN_A0;
// Status for is_key_valid
constexpr uint8_t LED_KEY_VALID = PIN_A5;
// To hack the controller
constexpr uint8_t HACK_ACTIVATE = PIN_A2;
// Random seed source
constexpr uint8_t SEED = PIN_A4;

// Keypad matrix
const byte ROWS = 2; // Two rows
const byte COLS = 2; // Two columns
constexpr uint8_t CODE_LEN = 4;
byte keys[ROWS][COLS] = { {1, 3}, {2, 4} }; // The return from keypress
byte rowPins[ROWS] = { PIN5, PIN6 }; // 2, 3
byte colPins[COLS] = { PIN7, 8 }; // 4, 5

// Instance declarations
MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 instance
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Shit for Auth
size_t name_len = 11;
byte valid_name[12] = {'C','a','r','l',' ','B','a','t','m','a','n','\0'};
constexpr byte valid_code[CODE_LEN] = { 1, 2, 3, 4 };

boolean is_current_attack(attacks_t attack) {
    return current_attack == attack;
}

// Output for door lock
void setDoorLock(boolean lock_door) {
    Serial.print(F("Setting door to "));
    String lock = lock_door ? "locked" : "unlocked";
    Serial.println(lock);

    if (is_current_attack(block_door_lock)) {
        Serial.println(F("Not sending door lock signal."));
    } else {
        digitalWrite(DOOR_LOCK, lock_door);
    }
}

// Helper function for reading card, exits early on error
boolean readCardData(byte *name, byte len) {
    byte block = 1;
   
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

    // Get key for authenticating the bank on card
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

// Helper function for writing, not used later
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


void signalHacked() {
    Serial.println(F("Oh, no. I've been hacked!"));
    
    randomSeed(analogRead(SEED)); // Seed

    current_attack = (attacks_t) random(0, ATTACKS_LEN);

    Serial.println(attack_labels[current_attack]);

    // Signal by flashing keyvalid led
    for (size_t i = 0; i < 5; i++) {
        digitalWrite(LED_KEY_VALID, HIGH);
        delay(200);
        digitalWrite(LED_KEY_VALID, LOW);
        delay(200);
    }
}