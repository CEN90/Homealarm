#include "controller.hpp"

unsigned long timeout; // Keep track after scan key

boolean armed = false;
boolean alarm = false;
boolean is_valid_key = false;
boolean is_valid_code = false;
boolean door_open = LOW;
boolean update_door = false;
boolean prev_door_state = false;

boolean hacked = false;

// Storage for keys pressed by the user
byte read_keypresse[CODE_LEN] = { 0, 0, 0, 0 };
byte key_press_index = 0; // Last index input

 
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);

    // Output LEDs
    pinMode(DOOR_LOCK, OUTPUT);
    pinMode(ALARM_OUTPUT, OUTPUT);
    pinMode(ARMED_OUTPUT, OUTPUT);
    pinMode(LED_KEY_VALID, OUTPUT);

    // Input
    pinMode(HACK_ACTIVATE, INPUT_PULLUP);
    pinMode(DOOR_SENSOR, INPUT_PULLUP);
    
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD
}

void loop() {
    hack();

    readDoorSensor();

    updateLED();

    if (update_door) {
        delay(DOOR_LOCK_TIME);
        setDoorLock(armed);
        update_door = false;
    }

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
    
    // If successful then reset and change armed status
    if (is_valid_key && is_valid_code) {
        resetKeypress();
        setArmedStatus();
        is_valid_key = false;
    }
    
    // Check if alarm is to be turned on last
    checkAlarmConditions();

    delay(POLL_TIME);
}

void hack() {
    if (digitalRead(HACK_ACTIVATE) && !hacked) {
        signalHacked();
        hacked = true;
    }
}

void updateLED() {
    digitalWrite(ARMED_OUTPUT, armed);
    digitalWrite(LED_KEY_VALID, is_valid_key);
}

void readDoorSensor() {
    auto curr_door_state = digitalRead(DOOR_SENSOR);

    if (curr_door_state != prev_door_state) {        
        prev_door_state = curr_door_state; // Want to print out despite being attacked

        if (curr_door_state) {
            Serial.println(F("Door opened"));
        } else {
            Serial.println(F("Door closed"));
        }

        if (is_current_attack(block_door_sensor)) {
            Serial.println(F("Ignoring it"));
        } else {
            door_open = curr_door_state;
        }
    }
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
    // Ignore alarm condition if hacked
    if (is_current_attack(block_alarm)) {
        Serial.println(F("No alarm signal is going out"));
        return;
    }

    alarm = true;
    Serial.println(F("ALARMA!"));
    digitalWrite(ALARM_OUTPUT, HIGH);
    digitalWrite(DOOR_LOCK, LOW); // If door is kicked in then it's no longer locked
}

void turnOffAlarm() {
    alarm = false;
    digitalWrite(ALARM_OUTPUT, LOW);
}

void setArmedStatus() {
    update_door = true;

    if (is_current_attack(block_armed)) {
        Serial.println(F("Alarm disabled, can't arm!"));
        return;
    }

    armed = !armed;
    turnOffAlarm(); // Always turn off alarm if user authenticates
    // update_door = true;
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

    // If hacked then all code is ok
    if (is_current_attack(block_auth)) {
        Serial.println(F("Not checking code"));
        return true;
    }

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

    // If hacked the do no auth
    if (is_current_attack(block_auth)) {
        Serial.println(F("Not checking card, approved!"));
        is_valid_key = true;
        resetKeypress();
        mfrc522.PCD_StopCrypto1();
        return;
    }

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
