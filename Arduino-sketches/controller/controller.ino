#include "controller.hpp"

constexpr uint8_t POLL_TIME = 10; 
constexpr uint8_t KEYPRESS_TIME = 15000; 

unsigned long timeout;

boolean armed = false;
boolean alarm = false;
boolean is_valid_key = false;
boolean is_valid_code = false;
boolean door_open = LOW;

byte read_keypresse[CODE_LEN] = { 0, 0, 0, 0 };
byte key_press_index = 0;

 
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD

    // Servo init
    lock_servo.attach(DOOR_LOCK);
    lock_servo.write(SERVO_POS_UNLOCKED); 
}

void loop() {
    readCard();

    // Invalidate key if timeout
    // if (millis() >= timeout) {
    //     is_valid_key = false;
    // }
    
    // Read keypad only if valid key
    if (is_valid_key) {
        readKeypad();
    }
    
    // If successful
    if (is_valid_key && is_valid_code) {
    // if (is_valid_code) {
        resetKeypress();
        setArmedStatus();
    }
    
    // Check if alarm is to be turned on last
    // checkAlarmConditions();
    
    delay(POLL_TIME);
}

void checkAlarmConditions() {
    alarm = armed && door_open;
}

void setArmedStatus() {
    alarm = false;
    armed = !armed;
    setDoorLock(armed);
}

void readKeypad() {
    byte key = kpd.getKey();

    // getKey always returns 0 if no key pressed, return early then
    if (!key) {
        return;
    }
    
    Serial.println(key);

    // Store read key input in arr
    if (key_press_index < CODE_LEN) {
        read_keypresse[key_press_index] = key;
        key_press_index += 1;
    }
    
    // If entire code is inputted
    if (key_press_index == CODE_LEN) {
        is_valid_code = checkCode();
    }
}

boolean checkCode() {
    boolean valid = true;

    for (size_t i = 0; i < CODE_LEN; i++) {
        if (read_keypresse[i] != valid_code[i])
            valid = false;
    }
    
    return valid;
}

// To reset all variables for handling keypad
void resetKeypress() {
    timeout = millis() + KEYPRESS_TIME;
    key_press_index = 0;
    is_valid_code = false;

    for (size_t i = 0; i < CODE_LEN; i++) {
        read_keypresse[i] = 0;
    } 
}

// Issue: works only once, need reset before reading card again!
void readCard() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte buffer[20];
    byte buffer_len = 20;

    if (readCardData(buffer, buffer_len)) {
        Serial.println((char*) buffer);

        if (strcmp((char *) buffer, (char *) valid_name) == 0) {
            is_valid_key = true;
            resetKeypress();
            Serial.println("Valid key found");
        } else {
            is_valid_key = false;
            Serial.println("Invalid key found");
        }
    }

    mfrc522.PCD_Reset();
}
