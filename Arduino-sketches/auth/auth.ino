int pin = PIN1;

void setup() {
    pinMode(pin, OUTPUT);
}

void loop() {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
}

// alarm -> Röd
// armed -> Gul
// door_locked -> Blå 
// is_valid_key -> Grön