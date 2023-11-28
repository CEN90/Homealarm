#include "states.hpp"

#define BAUD  115200

constexpr int ALARM_IN = 13;
constexpr int ARMED_IN = 12;
constexpr int DOOR_LOCK_IN = 11;
constexpr int KEY_VALID_IN = 10;
constexpr int DOOR_SENSOR_IN = 9;

constexpr int ALARM_OUT = PIN7;
constexpr int ARMED_OUT = PIN6;
constexpr int DOOR_LOCK_OUT = PIN5;
constexpr int KEY_VALID_OUT = PIN4;
constexpr int DOOR_SENSOR_OUT = PIN3;

constexpr int ERROR_STATE = -1;
constexpr int ERROR_LED = PIN2;
constexpr int POLL_TIME = 10;

boolean alarm_state = false;
boolean armed_state = false;
boolean door_lock_state = false;
boolean key_valid_state = false;
boolean door_sensor_state = false;

boolean prev_input_states[5] = { false };
boolean current_input_states[5] = { false };
constexpr int input_states_len = 5;

enum input_states {
    ALARM_STATE,
    ARMED_STATE,
    DOOR_LOCK_STATE,
    KEY_VALID_STATE,
    DOOR_SENSOR_STATE
};

void error(int current_state) {
    int s = transitions[current_state][Label];
    
    Serial.println(F("Something went wrong!"));
    Serial.print(F("Previous state: "));
    Serial.println(labels_string[s] + '\n');
    
    digitalWrite(ERROR_LED, HIGH);
}

void setPins() {
    pinMode(ALARM_IN, INPUT);
    pinMode(ARMED_IN, INPUT);
    pinMode(DOOR_LOCK_IN, INPUT);
    pinMode(KEY_VALID_IN, INPUT);
    pinMode(DOOR_SENSOR_IN, INPUT);

    pinMode(ALARM_OUT, OUTPUT);
    pinMode(ARMED_OUT, OUTPUT);
    pinMode(DOOR_LOCK_OUT, OUTPUT);
    pinMode(KEY_VALID_OUT, OUTPUT);
    pinMode(DOOR_SENSOR_OUT, OUTPUT);

    pinMode(ERROR_LED, OUTPUT);
    digitalWrite(ERROR_LED, LOW);
}

void readPinStates() {
    current_input_states[ALARM_STATE] = digitalRead(ALARM_IN);
    current_input_states[ARMED_STATE] = digitalRead(ARMED_IN);
    current_input_states[DOOR_LOCK_STATE] = digitalRead(DOOR_LOCK_IN);
    current_input_states[KEY_VALID_STATE] = digitalRead(KEY_VALID_IN);
    current_input_states[DOOR_SENSOR_STATE] = digitalRead(DOOR_SENSOR_IN);
}

boolean comparePinStates() {
    auto cmp = true;

    for (size_t i = 0; i < input_states_len; i++) {
        if (current_input_states[i] != prev_input_states[i]) {
            cmp = false;
        }

        prev_input_states[i] = current_input_states[i];
    }
    
    return cmp;
}

void updateLEDs() {
    digitalWrite(ALARM_OUT, current_input_states[ALARM_STATE]);
    digitalWrite(ARMED_OUT, current_input_states[ARMED_STATE]);
    digitalWrite(DOOR_LOCK_OUT, current_input_states[DOOR_LOCK_STATE]);
    digitalWrite(KEY_VALID_OUT, current_input_states[KEY_VALID_STATE]);
    digitalWrite(DOOR_SENSOR_OUT, current_input_states[DOOR_SENSOR_STATE]);
}

// void readPinStates() {
//     alarm_state = digitalRead(ALARM_IN);
//     armed_state = digitalRead(ARMED_IN);
//     door_lock_state = digitalRead(DOOR_LOCK_IN);
//     key_valid_state = digitalRead(KEY_VALID_IN);
//     door_sensor_state = digitalRead(DOOR_SENSOR_IN);
// }

// void updateLEDs() {
//     digitalWrite(ALARM_OUT, alarm_state);
//     digitalWrite(ARMED_OUT, armed_state);
//     digitalWrite(DOOR_LOCK_OUT, door_lock_state);
//     digitalWrite(KEY_VALID_OUT, key_valid_state);
//     digitalWrite(DOOR_SENSOR_OUT, door_sensor_state);
// }

template <typename T>
Print& operator<<(Print& printer, T value) {
    printer.print(value);
    return printer;
}

int findState(int searched, int* startpos, int* endpos) {
    *startpos = searched;

    while (transitions[*startpos][From] != searched && *startpos < total_states) {
        *startpos += 1;
    }

    *endpos = *startpos;
    
    while (transitions[*endpos + 1][From] == searched && *endpos < total_states) {
        *endpos += 1;
    }
    
    int len = *endpos - *startpos + 1;
    return len > 0 ? len : -1; // return -1 if error
}