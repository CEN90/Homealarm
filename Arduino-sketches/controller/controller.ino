#include "controller.hpp"

unsigned long timeout; // Keep track after scan key

boolean armed = false;
boolean alarm = false;
boolean is_valid_key = false;
boolean is_valid_code = false;
boolean door_open = LOW;

// Storage for keys pressed by the user
byte read_keypresse[CODE_LEN] = { 0, 0, 0, 0 };
byte key_press_index = 0; // Last index input

 
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD

    // Servo init
    lock_servo.attach(DOOR_LOCK);
    lock_servo.write(SERVO_POS_UNLOCKED); 

    pinMode(ALARM_OUTPUT, OUTPUT);
    pinMode(ARMED_OUTPUT, OUTPUT);
    pinMode(DOOR_SENSOR, INPUT_PULLUP);
}

void loop() {
    door_open = digitalRead(DOOR_SENSOR); // testing only
    // door_open = !digitalRead(DOOR_SENSOR); // Has to reversed, INPUT_PULLUP => active LOW

    digitalWrite(ARMED_OUTPUT, armed);

    // Only read if no valid key scanned 
    if (!is_valid_key) {
        readCard();
    }

    // Invalidate key if timeout
    checkTimeout();

    // Read keypad only if valid key
    if (is_valid_key) {
        readKeypad();
    }
    
    // If successful
    if (is_valid_key && is_valid_code) {
        resetKeypress();
        setArmedStatus();
        is_valid_key = false;
    }
    
    // Check if alarm is to be turned on last
    checkAlarmConditions();

    delay(POLL_TIME);
}

void checkTimeout() {
    if (is_valid_key && millis() >= timeout) {
        is_valid_key = false;
        Serial.println(F("Timeout"));
    }
}

void checkAlarmConditions() {
    if (!alarm && armed && door_open) {
        ALARMA(); // Turn on alarm if bad actor on tv
    }
}

void ALARMA() {
    alarm = true;
    Serial.println(F("ALARMA!"));
    digitalWrite(ALARM_OUTPUT, HIGH);
}

void turnOffAlarm() {
    alarm = false;
    digitalWrite(ALARM_OUTPUT, LOW);
}

void setArmedStatus() {
    armed = !armed;
    turnOffAlarm(); // Always turn off alarm if user authenticates
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
    
    // If entire code is input
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


void readCard() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte buffer[20];
    byte buffer_len = 20;

    // If successful read of card
    if (readCardData(buffer, buffer_len)) {
        Serial.println((char*) buffer);

        if (strcmp((char *) buffer, (char *) valid_name) == 0) {
            is_valid_key = true;
            resetKeypress();
            Serial.println(F("Valid key found"));
        } else {
            is_valid_key = false;
            Serial.println(F("Invalid key found"));
            delay(ERROR_KEY_TIME); // Wait to avoid ugly loop
        }
    }

    mfrc522.PCD_StopCrypto1(); // Or else no new card can be scanned
}
