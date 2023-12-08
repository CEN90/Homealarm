#include "monitor.hpp"

int current_state = 0;
int next_state = 0;

boolean error_state = false;

/* Not used for now */
boolean timer_on = false;
unsigned long timer = 0;

int read_inputs = 0;
int prev_inputs = 0;

int possible_states_len, startpos, endpos;

void setup() {
    Serial.begin(BAUD);

    setPins();

    next_state = start_state;
}

void loop() {   
    read_inputs = readInputs();
    updateLEDs(); // To make it easier to follow

    // If timer is turned on and has run out, do something
    if (timer_on && timer < millis())
    {
        Serial.println(F("Time ran out"));
    }
    
    // Do nothing if pin states is same
    if (read_inputs == prev_inputs) {
        delay(POLL_TIME);
        return;
    }
    
    printdebugInput();
    
    // Error out early
    if (error_state) {
        error(current_state);
        return;
    }

    current_state = next_state;

    possible_states_len = findState(current_state, &startpos, &endpos);
    error_state = possible_states_len == ERROR_STATE; // Do nothing if error

    if (error_state) {
        error(current_state);
        return;
    }

    printPossibleChoices();

    // Actual comparison here!
    next_state = compare();
    
    prev_inputs = read_inputs;
    delay(POLL_TIME);
}

int compare() {
    // For every possible state
    for (size_t i = 0; i < possible_states_len; i++)
    {
        auto state_label = transitions[startpos + i][Label];
        auto transition_to = transitions[startpos + i][To];

        // Compare the states possible inputs
        auto is_match = expected_inputs[state_label].inputs_len == 0; // If 0 then for wont run
        for (size_t j = 0; j < expected_inputs[state_label].inputs_len; j++) {
            if (read_inputs == expected_inputs[state_label].valid_inputs[i]) {
                is_match = true;
                // Serial.print("Kompilator Kompis Kim");
                // Serial.println(output_strings[state_label]);
            }
        }

        if (is_match) {
            return transition_to;
        }
    }

    return ERROR_STATE;
}

// void printStateOutput(int state) {
//     Serial.println(output_strings[state]);
// }

void printPossibleChoices() { 
    Serial << "Current state:" << " " << current_state << ", possible labels -> ";   

    for (size_t i = 0; i < possible_states_len; i++) {
        int s = transitions[startpos + i][Label];
        Serial << labels_string[s] << " ";
    }
    
    Serial.println("");
}