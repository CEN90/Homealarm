#include "monitor.hpp"
#define BAUD  115200

int current_state = 0;
int next_state = 0;

boolean error_state = false;
boolean tau = false;

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
        Serial.println("Time ran out");
    }
    
    // Do nothing if pin states is same
    if ((read_inputs == prev_inputs && !tau) || error_state) {
        delay(POLL_TIME);
        return;
    }
    
    printdebugInput(); // Print input to terminal
    
    // Error out early
    if (next_state == ERROR_STATE) {
        error_state = true;
        error(startpos);
        // error(current_state);
        return;
    }

    current_state = next_state;

    possible_states_len = findState(current_state, &startpos, &endpos);

    // If no new state could be found then go to ERROR_STATE
    if (possible_states_len == ERROR_STATE) {
        next_state = ERROR_STATE;
        return;
    }

    printPossibleChoices(current_state, startpos, possible_states_len);

    // Actual comparison here!
    next_state = compare(startpos, possible_states_len);
    
    prev_inputs = read_inputs;
    delay(POLL_TIME);
}

int compare(int start, int len) {
    printInput(read_inputs);
    // For every possible state
    for (size_t i = 0; i < len; i++)
    {
        int state_label = transitions[start + i][Label];
        int transition_to = transitions[start + i][To];
        int inputs_len = expected_inputs[state_label].inputs_len;
        
        // Compare the states possible inputs
        boolean is_match = false; // If then skip for

        if (inputs_len == 0) {
            tau = true;
            is_match = true;
            Serial.println(F("Encountered a tau transition"));
        } else {
            tau = false;
        }

        for (size_t j = 0; j < inputs_len; j++) {
            if (read_inputs == expected_inputs[state_label].valid_inputs[j]) {
                printStateOutput(state_label);
                is_match = true;
            }
        }

        if (is_match) {
            return transition_to;
        }
    }

    // Serial.println(read_inputs);
    return ERROR_STATE;
}
