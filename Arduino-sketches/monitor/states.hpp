constexpr int total_transitions = 9;
constexpr int total_states = 13;
constexpr int start_state = 7;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setunarmed,
    controller_doorclosed,
    door_lockdoor,
    controller_alarma,
    door_unlockdoor,
    controller_setarmed,
};

String labels_string[7] = {
    "controller.dooropened",
    "controller.setunarmed",
    "controller.doorclosed",
    "door.lockdoor",
    "controller.alarma",
    "door.unlockdoor",
    "controller.setarmed",
};

int transitions[14][4] = {
    { 0, 0, -1, 8 },
    { 0, 1, -1, 7 },
    { 1, 2, -1, 0 },
    { 1, 1, -1, 5 },
    { 2, 3, -1, 4 },
    { 3, 4, -1, 1 },
    { 4, 0, -1, 3 },
    { 4, 1, -1, 6 },
    { 5, 2, -1, 7 },
    { 6, 5, -1, 7 },
    { 7, 0, -1, 5 },
    { 7, 6, -1, 2 },
    { 8, 2, -1, 0 },
};
