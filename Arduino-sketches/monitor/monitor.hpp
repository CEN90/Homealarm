#include "EdgeReader.hpp"

constexpr uint8_t ALARM_INPUT = 9;
constexpr uint8_t ARMED_INPUT = 10;
constexpr uint8_t DOOR_LOCK_INPUT = 11;
constexpr uint8_t KEY_VALID_INPUT = 12;
constexpr uint8_t DOOR_SENSOR_INPUT = 13;

constexpr uint8_t 

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