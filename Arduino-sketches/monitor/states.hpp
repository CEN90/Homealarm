constexpr int total_transitions = 12;
constexpr int total_states = 16;
constexpr int start_state = 5;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setunarmed,
    time,
    controller_doorclosed,
    door_unlockdoor,
    controller_setarmed,
    controller_alarma,
    door_lockdoor,
};

String labels_string[8] = {
    "controller.dooropened",
    "controller.setunarmed",
    "time",
    "controller.doorclosed",
    "door.unlockdoor",
    "controller.setarmed",
    "controller.alarma",
    "door.lockdoor",
};

int transitions[17][4] = {
    { 0, 0, -1, 7 },
    { 0, 1, -1, 3 },
    { 1, 2, 1, 0 },
    { 2, 3, -1, 9 },
    { 2, 1, -1, 6 },
    { 3, 4, -1, 5 },
    { 4, 2, 1, 11 },
    { 5, 0, -1, 8 },
    { 5, 5, -1, 10 },
    { 6, 3, -1, 5 },
    { 7, 6, -1, 2 },
    { 8, 2, 1, 6 },
    { 9, 0, -1, 4 },
    { 9, 1, -1, 3 },
    { 10, 7, -1, 1 },
    { 11, 3, -1, 9 },
};
