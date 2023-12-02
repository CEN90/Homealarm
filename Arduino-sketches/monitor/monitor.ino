#include "monitor.hpp"

int current_state = 0;
int next_state = 0;
int sleep = POLL_TIME;

boolean error_state = false;
boolean check_door_next = false;

int states_len, startpos, endpos;

void setup() {
    Serial.begin(BAUD);

    setPins();

    next_state = start_state;
}

void loop() {   
    readPinStates();
    updateLEDs(); // To make it easier to follow

    // Do nothing if pin states is same
    if (comparePinStates() && !check_door_next) {
        delay(sleep);
        return;
    }
    
    check_door_next = false;
    
    for (auto &&i : current_input_states) { // Simple debug output
        Serial << i << ' ';
    }
    Serial << '\n';
    
    // Error out early
    if (error_state) {
        error(current_state);
        return;
    }

    current_state = next_state;

    states_len = findState(current_state, &startpos, &endpos);
    error_state = states_len == ERROR_STATE; // Do nothing if error

    if (!error_state) { // Also debug output
        printPossibleChoices(startpos, states_len);
        Serial << "startpos: " << startpos << " endpos: " << endpos << '\n';
        stateSwitch(states_len, startpos);
        Serial << '\n';
    }
    
    delay(sleep);
}

void stateSwitch(int states_len, int startpos) {
    int n_state = ERROR_STATE; // Gets replaced if a good state found

    for (size_t i = 0; i < states_len; i++) {
        switch (transitions[startpos + i][Label]) {
            case door_opendoor: // Expecting open door
                if (current_input_states[DOOR_SENSOR_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Door opened"));
                }
                break;

            case door_closedoor: // Expecting closed door
                if (!current_input_states[DOOR_SENSOR_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Door closed"));
                }
                break;

            case door_lockdoor: // Expecting a docked door
                if (current_input_states[DOOR_LOCK_STATE] && !current_input_states[DOOR_SENSOR_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Door locked"));
                }
                break;

            case door_unlockdoor:
                if (!current_input_states[DOOR_LOCK_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Door unlocked"));
                }
                break;
            
            case controller_setarmed:
                if (current_input_states[ARMED_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Alarm set armed"));
                }
                break;

            case controller_setdoorstatus:
                Serial << "door_status " << door_status << "\n";
                if (transitions[startpos + i][Value] == door_status)
                {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Door status changed"));
                }
                
                break;

            case controller_setunarmed:
                if (!current_input_states[ARMED_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("Alarm set unarmed"));
                }
                break;

            case controller_alarma:
                if (current_input_states[ALARM_STATE]) {
                    n_state = transitions[startpos + i][To];
                    Serial.println(F("ALARMA!"));
                }
                break;
            
            default:
                break;
        }

        if (n_state != ERROR_STATE) { // If a match was found then go with it
            break;
        }
    }

    if (n_state == ERROR_STATE) { // We want to stop loop if encounter error
        error_state = true;
    }

    int next_start, next_end;
    findState(n_state, &next_start, &next_end); // Translate a state id to its index in transisions array
    if (transitions[next_start][Label] == controller_setdoorstatus) { // Treating setdoorstatus as a tau
        check_door_next = true;
        Serial.println("Checking door next");
    }

    next_state = n_state;  
}

void printPossibleChoices(int start, int len) {
    int s = transitions[start][Label];
    // int s = transitions[current_state][Label];
    Serial << "Current state:" << " " << current_state << " -> " << labels_string[s] << "\n";   
}