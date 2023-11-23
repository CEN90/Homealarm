// #include "EdgeReader.hpp"
#include "states.hpp"

#define BAUD  115200

constexpr uint8_t ALARM_IN = 9;
constexpr uint8_t ARMED_IN = 10;
constexpr uint8_t DOOR_LOCK_IN = 11;
constexpr uint8_t KEY_VALID_IN = 12;
constexpr uint8_t DOOR_SENSOR_IN = 13;

constexpr uint8_t ALARM_OUT = PIN3;
constexpr uint8_t ARMED_OUT = PIN4;
constexpr uint8_t DOOR_LOCK_OUT = PIN5;
constexpr uint8_t KEY_VALID_OUT = PIN6;
constexpr uint8_t DOOR_SENSOR_OUT = PIN7;

constexpr uint8_t ERROR_STATE = -1;
constexpr uint8_t ERROR_LED = PIN2;
constexpr uint8_t POLL_TIME = 10;

boolean alarm_state = false;
boolean armed_state = false;
boolean door_lock_state = false;
boolean key_valid_state = false;
boolean door_sensor_state = false;

// EdgeReader edgereader = EdgeReader();
// Transitions transitions;

// String state_file = "states_alarm.aut";
// constexpr uint8_t STATES_LEN = 9;
// String names[] = {
//     "other",
//     "time",
//     "controller.alarma",
//     "controller.setArmed",
//     "controller.setunarmed",
//     "door.lockDoor",
//     "door.unlockDoor", 
//     "door.openDoor()", 
//     "door.closeDoor()", 
// };

void error() {
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
    alarm_state = digitalRead(ALARM_IN);
    armed_state = digitalRead(ARMED_IN);
    door_lock_state = digitalRead(DOOR_LOCK_IN);
    key_valid_state = digitalRead(KEY_VALID_IN);
    door_sensor_state = digitalRead(DOOR_SENSOR_IN);
}

void updateLEDs() {
    digitalWrite(ALARM_OUT, alarm_state);
    digitalWrite(ARMED_OUT, armed_state);
    digitalWrite(DOOR_LOCK_OUT, door_lock_state);
    digitalWrite(KEY_VALID_OUT, key_valid_state);
    digitalWrite(DOOR_SENSOR_OUT, door_sensor_state);
}