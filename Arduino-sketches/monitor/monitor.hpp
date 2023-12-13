#include "states.hpp"

// Input Arduino Uno
// constexpr int ALARM = 13;
// constexpr int ARMED = 12;
// constexpr int DOOR_LOCK = 11;
// constexpr int DOOR_SENSOR = 9;

// Input Arduino Mega pinout
constexpr int ALARM = PIN4;
constexpr int ARMED = PIN3;
constexpr int DOOR_LOCK = PIN2;
constexpr int DOOR_SENSOR = 51;

constexpr int input_pins_len = 4;
constexpr int input_pins[input_pins_len] = { ALARM, ARMED, DOOR_LOCK, DOOR_SENSOR };

// Error output
constexpr int ERROR_STATE = -1;
constexpr int POLL_TIME = 10;

struct inputs_t {
    int state;
    int inputs_len;
    int valid_inputs[input_pins_len + 2];
};

inputs_t expected_inputs[8] = {
    { controller_dooropened, 4, { 8, 9, 11, 14 } },
    { controller_setunarmed, 2, { 0, 4 } },
    { controller_doorclosed, 3, { 0, 1, 3 } },
    { controller_alarmaoff, 1, { 2 }},
    { door_lockdoor, 1, { 6 } },
    { door_unlockdoor, 1, { 0 } },
    { controller_alarma, 3, { 1, 3, 11 } },
    { controller_setarmed, 3, { 2, 4, 6 } },
};

String output_strings[8] = {
    "Door opened",
    "Alarm set unarmed",
    "Door closed",
    "ALARMA off",
    "Door locked",
    "Door unlocked",
    "ALARMA!",
    "Alarm set armed",
};

void error(int current_state) {
    int s = transitions[current_state][Label];
    
    Serial.println(F("Something went wrong!"));
    Serial.print(F("Previous state -> "));
    Serial.println(current_state + '\n');
    Serial.println(F("Failed to match input to a state, entering following mode instead!\n"));
}

void setPins() {
    for (size_t i = 0; i < input_pins_len; i++) {
        pinMode(input_pins[i], INPUT);
    }
}

int readInputs() {
    int result = 0;
    for (int i = 0; i < input_pins_len; i++) {
        result += digitalRead(input_pins[i]) << i;
    }

    return result;
}

void printInput(int input) {
    Serial.print("Input: ");
    Serial.println(input);
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