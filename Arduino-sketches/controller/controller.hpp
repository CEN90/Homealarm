#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
// #include <Servo.h>

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
constexpr uint8_t DOOR_SENSOR = PIN_A0;
// constexpr uint8_t DOOR_LOCK_SERVO = PIN_WIRE_SCL;
constexpr uint8_t DOOR_LOCK = PIN2;
// Servo motor limits
constexpr uint8_t SERVO_POS_LOCKED = 90; 
constexpr uint8_t SERVO_POS_UNLOCKED = 0; 
// Status for is_key_valid
constexpr uint8_t LED_KEY_VALID = PIN_A5;

// Keypad matrix
const byte ROWS = 2; // Two rows
const byte COLS = 2; // Two columns
constexpr uint8_t CODE_LEN = 4;
byte keys[ROWS][COLS] = { {1, 3}, {2, 4} }; // The return from keypress
byte rowPins[ROWS] = { PIN5, PIN6 }; // 2, 3
byte colPins[COLS] = { PIN7, 8 }; // 4, 5
// byte rowPins[ROWS] = { PIN2, PIN3 }; // 2, 3
// byte colPins[COLS] = { PIN4, PIN5 }; // 4, 5

// Instance declarations
MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 instance
// Servo lock_servo; // Servo instance
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Shit for Auth
size_t name_len = 11;
byte valid_name[12] = {'C','a','r','l',' ','B','a','t','m','a','n','\0'};
constexpr byte valid_code[CODE_LEN] = { 1, 2, 3, 4 };

// Controls the servo motor
void setDoorLock(boolean lock_door) {
    // int new_pos = lock_door ? SERVO_POS_LOCKED : SERVO_POS_UNLOCKED;
    // lock_servo.write(new_pos);
    
    digitalWrite(DOOR_LOCK, lock_door);

    Serial.print(F("Setting door to "));
    String lock = lock_door ? "locked" : "unlocked";
    Serial.println(lock);
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