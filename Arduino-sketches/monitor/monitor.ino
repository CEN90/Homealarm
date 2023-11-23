#include "monitor.hpp"

int current_state = 0;
int next_state = 0;
int sleep = 0;

void setup() {
    Serial.begin(BAUD);

    setPins();
    
    // edgereader.readTransitions(state_file, names, STATES_LEN);
    // transitions = edgereader.edges;
    // next_state = edgereader.start_state;

    next_state = ERROR_STATE;
}

void loop() {   
    readPinStates();
    updateLEDs(); // To make it easier to follow

    current_state = next_state;

    if (current_state == ERROR_STATE) {
        Serial.println(F("Something went wrong!"));
        error();
    }
    
    sleep = POLL_TIME;

    

    delay(sleep);
}

