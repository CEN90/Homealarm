#include "states.hpp"

// Input Arduino Uno
// constexpr int ALARM = 13;
// constexpr int ARMED = 12;
// constexpr int DOOR_LOCK = 11;
// constexpr int DOOR_SENSOR = 9;

// Input Arduino Mega pinout
constexpr int ALARM = PIN7;
constexpr int ARMED = PIN6;
constexpr int DOOR_LOCK = PIN5;
constexpr int DOOR_SENSOR = PIN4;

constexpr int input_pins_len = 4;
constexpr int input_pins[input_pins_len] = { ALARM, ARMED, DOOR_LOCK, DOOR_SENSOR };

struct inputs_t {
    int state;
    int inputs_len;
    int valid_inputs[input_pins_len + 2];
};

struct input_t {
    int temp_main;
    int temp_aux;
    int commands;
    boolean unchanged;
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

void readInputs(input_t *prev) {
    int result = 0;
    for (int i = 0; i < input_pins_len; i++) {
        result += digitalRead(input_pins[i]) << i;
    }

    if (prev->commands = result) {
        prev->unchanged = true;
    } else {
        prev->commands = result;
        prev->unchanged = false;
    }
    
}

boolean is_expected_input(input_t current, inputs_t state) {
    boolean match = false;
    
    for (size_t i = 0; i < state.inputs_len; i++) {
        if (current.commands == state.valid_inputs[i]) {
            match = true;
        }
    }

    return match;
}
