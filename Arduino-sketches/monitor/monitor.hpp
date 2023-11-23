#include "EdgeReader.hpp"

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