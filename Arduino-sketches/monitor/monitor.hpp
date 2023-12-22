#include "tinytwin.hpp"

// // Error output
constexpr int ERROR_STATE = -1;
constexpr int POLL_TIME = 10;


void error(int current_state) {
    int s = transitions[current_state][Label];
    
    Serial.println(F("Something went wrong!"));
    Serial.print(F("Previous state -> "));
    Serial.println(current_state + '\n');
    Serial.println(F("Failed to match input to a state, entering following mode instead!\n"));
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