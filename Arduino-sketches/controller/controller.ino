#define DOOR_SENSOR     PIN0
#define DOOR_LOCK       PIN1
#define AUTH_IN         PIN3

boolean armed = false;
boolean alarm = false;
boolean door_open = false;
boolean door_locked = false;

void setup() {
    pinMode(DOOR_SENSOR, INPUT);
    pinMode(DOOR_LOCK, OUTPUT);
    pinMode(AUTH_IN, INPUT);
}

void arm() {
    alarm = false;
    door_locked = true;
    armed = true;
}

void unarm() {
    alarm = false;
    door_locked = false;
    armed = false;
}

void loop() {
    boolean auth_ok = digitalRead(AUTH_IN);
    door_open = digitalRead(DOOR_SENSOR);

    if (auth_ok)
    {
        if (armed)
        {
            unarm();
        } 
        
    }
    
    //maybe, maybe noTone(pin);
    if (armed && door_open)
    {
        alarm = true; 
        //BEEEEPBOOOOOOOOOOOOOOP
    }
}