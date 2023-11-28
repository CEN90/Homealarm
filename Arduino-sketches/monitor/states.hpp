constexpr int total_transitions = 21;
constexpr int total_states = 28;
constexpr int start_state = 0;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    door_opendoor,
    controller_setarmed,
    door_lockdoor,
    controller_setdoorstatus,
    controller_setunarmed,
    door_unlockdoor,
    controller_alarma,
    door_closedoor,
};

String labels_string[8] = {
    "door.opendoor",
    "controller.setarmed",
    "door.lockdoor",
    "controller.setdoorstatus",
    "controller.setunarmed",
    "door.unlockdoor",
    "controller.alarma",
    "door.closedoor",
};

int transitions[29][4] = {
    { 0, 0, -1, 20 },
    { 0, 1, -1, 10 },
    { 0, 2, -1, 9 },
    { 1, 3, -1, 12 },
    { 2, 4, -1, 1 },
    { 3, 5, -1, 1 },
    { 4, 4, -1, 3 },
    { 4, 5, -1, 2 },
    { 5, 6, -1, 4 },
    { 6, 6, -1, 4 },
    { 7, 3, -1, 6 },
    { 8, 3, -1, 7 },
    { 9, 1, -1, 8 },
    { 10, 2, -1, 8 },
    { 11, 3, -1, 18 },
    { 12, 0, -1, 19 },
    { 13, 4, -1, 11 },
    { 14, 5, -1, 11 },
    { 15, 4, -1, 14 },
    { 15, 5, -1, 13 },
    { 16, 1, -1, 15 },
    { 17, 2, -1, 15 },
    { 18, 1, -1, 17 },
    { 18, 2, -1, 16 },
    { 19, 7, -1, 18 },
    { 20, 3, -1, 19 },
};
