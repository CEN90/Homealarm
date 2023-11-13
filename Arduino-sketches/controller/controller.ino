#include <Servo.h>

#define DOOR_SENSOR         PIN4
#define DOOR_LOCK           PIN7
#define AUTH_IN             PIN2

#define SERVO_POS_LOCKED    90
#define SERVO_POS_UNLOCKED  0

Servo lock_servo;

boolean armed = false;
boolean alarm = false;
boolean auth_ok = false;
boolean door_open = false;

boolean prev_door_state = false;

void setup() {
    pinMode(AUTH_IN, INPUT); 
    pinMode(DOOR_SENSOR, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);

    lock_servo.attach(DOOR_LOCK);
    lock_servo.write(SERVO_POS_UNLOCKED);   
}

boolean setDoorLock(boolean lock_door) {
    int new_pos = SERVO_POS_UNLOCKED;

    if (lock_door) {
        new_pos = SERVO_POS_LOCKED;
    }

    lock_servo.write(new_pos);   

    return lock_door;
}

void loop() {
    // auth_ok = digitalRead(AUTH_IN);
    door_open = digitalRead(DOOR_SENSOR);

    // if (door_open && !prev_door_state) {
    //   prev_door_state = door_open;
    // } else {
    //   prev_door_state = setDoorLock(door_open);
    // }

    setDoorLock(door_open);

    // Authentication branch
    // if (auth_ok) {
    //     alarm = false; // Always turn off alarm when authenticated

    //     armed = !armed; // Flip armed status
    //     setDoorLock(armed); // Door lock status should follow armed
    // }
    
    // // Alarm branch
    // if (armed && door_open) {
    //     alarm = true; 
    // }

    delay(10);
}