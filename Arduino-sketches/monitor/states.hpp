constexpr int total_transitions = 9;
constexpr int total_states = 13;
constexpr int start_state = 5;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setunarmed,
    controller_doorclosed,
    door_lockdoor,
    door_unlockdoor,
    controller_alarma,
    controller_setarmed,
};

String labels_string[7] = {
    "controller.dooropened",
    "controller.setunarmed",
    "controller.doorclosed",
    "door.lockdoor",
    "door.unlockdoor",
    "controller.alarma",
    "controller.setarmed",
};

int transitions[14][4] = {
    { 0, 0, -1, 8 },
    { 0, 1, -1, 3 },
    { 1, 2, -1, 0 },
    { 1, 1, -1, 6 },
    { 2, 3, -1, 7 },
    { 3, 4, -1, 5 },
    { 4, 5, -1, 1 },
    { 5, 0, -1, 6 },
    { 5, 6, -1, 2 },
    { 6, 2, -1, 5 },
    { 7, 0, -1, 4 },
    { 7, 1, -1, 3 },
    { 8, 2, -1, 0 },
};
