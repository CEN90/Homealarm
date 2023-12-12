#include "states.hpp"

// Input Arduino Uno
// constexpr int ALARM_IN = 13;
// constexpr int ARMED_IN = 12;
// constexpr int DOOR_LOCK_IN = 11;
// constexpr int KEY_VALID_IN = 10;
// constexpr int DOOR_SENSOR_IN = 9;

// Output Arduino Uno pinout
// constexpr int ALARM_OUT = PIN7;
// constexpr int ARMED_OUT = PIN6;
// constexpr int DOOR_LOCK_OUT = PIN5;
// constexpr int KEY_VALID_OUT = PIN4;
// constexpr int DOOR_SENSOR_OUT = PIN3;

constexpr int input_pins_len = 4;
constexpr int input_pins[input_pins_len] = { PIN4, PIN3, PIN2, 51 };

// Input Arduino Mega pinout
constexpr int ALARM_IN = PIN4;
constexpr int ARMED_IN = PIN3;
constexpr int DOOR_LOCK_IN = PIN2;
// constexpr int KEY_VALID_IN = 50;
constexpr int DOOR_SENSOR_IN = 51;

// Output Arduino Uno pinout
constexpr int ALARM_OUT = PIN7;
constexpr int ARMED_OUT = PIN6;
constexpr int DOOR_LOCK_OUT = PIN5;
constexpr int KEY_VALID_OUT = PIN4;
constexpr int DOOR_SENSOR_OUT = PIN3;

// Output config
constexpr int ERROR_LED_PRESENT = 0;
constexpr int OUTPUT_PRESENT = 0;

// Error output
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

// enum door_status {
//     CLOSED_UNLOCKED,
//     CLOSER_LOCKED,
//     OPEN_UNLOCKED,
//     OPEN_LOCKED // Can't happen!
// };
// int door_status = CLOSED_UNLOCKED;

struct inputs_t {
    int state;
    int inputs_len;
    int valid_inputs[input_pins_len + 2];
};

inputs_t expected_inputs[8] = {
    { controller_dooropened, 3, { 8, 9, 11 } },
    { controller_setunarmed, 1, { 0 } },
    { time, 0, { 0 } },
    { controller_doorclosed, 2, { 0, 1 } },
    { door_unlockdoor, 1, { 0 } },
    { controller_setarmed, 3, { 1, 4, 6 } },
    { controller_alarma, 3, { 1, 3, 11 } },
    { door_lockdoor, 1, { 6 } },
};

// inputs_t expected_inputs[8] = {
//     { controller_dooropened, 4, { 16, 19, 22, 23 } },
//     { controller_setunarmed, 4, { 0, 10, 16, 26 } },
//     { time, 0, { 0 } },
//     { controller_doorclosed, 4, { 0, 3, 8, 19 } },
//     { door_unlockdoor, 3, { 0, 16, 24 } },
//     { controller_setarmed, 3, { 6, 12, 14 } },
//     { controller_alarma, 4, { 3, 23, 19, 32 } },
//     { door_lockdoor, 2, { 6, 24 } },
// };

String output_strings[9] = {
    "Door opened",
    "Alarm set unarmed",
    "time",
    "Door closed",
    "Door unlocked",
    "Alarm set armed",
    "ALARMA!",
    "Door locked",
};

void printInput(int input) {
    Serial.print("Input: ");
    Serial.println(input);
}

void error(int current_state) {
    int s = transitions[current_state][Label];
    
    Serial.println(F("Something went wrong!"));
    Serial.print(F("Previous state -> "));
    Serial.println(current_state + '\n');
    
    if (ERROR_LED_PRESENT) {
        digitalWrite(ERROR_LED, HIGH);
    }
}

void setPins() {
    for (size_t i = 0; i < input_pins_len; i++) {
        pinMode(input_pins[i], INPUT);
    }

    if (OUTPUT_PRESENT) {
        pinMode(ALARM_OUT, OUTPUT);
        pinMode(ARMED_OUT, OUTPUT);
        pinMode(DOOR_LOCK_OUT, OUTPUT);
        pinMode(KEY_VALID_OUT, OUTPUT);
        pinMode(DOOR_SENSOR_OUT, OUTPUT);
    }
    
    if (ERROR_LED_PRESENT) {
        pinMode(ERROR_LED, OUTPUT);
        digitalWrite(ERROR_LED, LOW);   
    }
}

void updateLEDs() {
    if (OUTPUT_PRESENT) {
        digitalWrite(ALARM_OUT, current_input_states[ALARM_STATE]);
        digitalWrite(ARMED_OUT, current_input_states[ARMED_STATE]);
        digitalWrite(DOOR_LOCK_OUT, current_input_states[DOOR_LOCK_STATE]);
        digitalWrite(KEY_VALID_OUT, current_input_states[KEY_VALID_STATE]);
        digitalWrite(DOOR_SENSOR_OUT, current_input_states[DOOR_SENSOR_STATE]);
    }
}

int readInputs() {
    int result = 0;
    for (int i = 0; i < input_pins_len; i++) {
        result += digitalRead(input_pins[i]) << i;
    }

    return result;
}

void printdebugInput() {
    for (auto &&i : current_input_states) { // Simple debug output
        Serial << i << ' ';
    }
    Serial << '\n';
}

void printState(int state) {
    Serial.print(labels_string[state] + ": ");
}

void printStateOutput(int state) {
    Serial.println(output_strings[state] + "\n");
}

template <typename T>
Print& operator<<(Print& printer, T value) {
    printer.print(value);
    return printer;
}

void printPossibleChoices(int current, int start, int len) { 
    Serial << "Current state:" << " " << current << ", possible labels -> ";   

    for (size_t i = 0; i < len; i++) {
        int s = transitions[start + i][Label];
        Serial << labels_string[s] << " ";
    }
    
    Serial.println("");
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
    return len > 0 ? len : ERROR_STATE; // return -1 if error
}