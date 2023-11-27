#include "monitor.hpp"

uint8_t current_state = 0;
uint8_t next_state = 0;
uint8_t sleep = 0;

void setup() {
    Serial.begin(BAUD);

    setPins();

    next_state = start_state;
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

    switch (transitions[current_state][Label])
    {
    case controller_setcurrent:
        /* code */
        break;
    
    default:
        break;
    }
    

    delay(sleep);
}

