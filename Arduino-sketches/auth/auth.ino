constexpr uint8_t pin_len = 5;
constexpr uint8_t pin_input[pin_len] = { 9, 10, 11, 12, 13 };
constexpr uint8_t pin_led[pin_len] = { 3, 4, 5, 6, 7 };

constexpr uint8_t sleep = 1;

void setup() {
    for (size_t i = 0; i < pin_len; i++)
    {
        pinMode(pin_input[i], INPUT);
        pinMode(pin_led[i], OUTPUT);
    }
}

void loop() {
    for (size_t i = 0; i < pin_len; i++)
    {
        digitalWrite(pin_led[i], digitalRead(pin_input[i]));
    }
    
    delay(sleep);
}

// alarm -> Röd
// armed -> Gul
// door_locked -> Blå 
// is_valid_key -> Grön