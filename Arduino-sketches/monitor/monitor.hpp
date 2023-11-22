#include "EdgeReader.hpp"

// Alarma!
constexpr uint8_t ALARM_OUTPUT = 8;
constexpr uint8_t ARMED_OUTPUT = PIN6;
// Door relevant ports
constexpr uint8_t DOOR_SENSOR = PIN_WIRE_SDA;
constexpr uint8_t DOOR_LOCK = PIN7;

constexpr uint8_t ERROR_STATE = -1;
constexpr uint8_t POLL_TIME = 10;


EdgeReader edgereader = EdgeReader();
Transitions transitions;

String state_file = "states_alarm.aut";
constexpr uint8_t STATES_LEN = 6;
String names[] = {
        "other",
        "time",
        "controller.armed",
        "controller.alarma",
        "door.setcooling",
        "hc_unit.switchoff"
};

void setPins() {

}