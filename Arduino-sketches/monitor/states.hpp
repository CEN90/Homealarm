constexpr int total_transitions = 17;
constexpr int total_states = 21;
constexpr int start_state = 6;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setkeyvalid,
    time,
    controller_doorclosed,
    door_unlockdoor,
    controller_setunarmed,
    controller_alarma,
    controller_setarmed,
    door_lockdoor,
};

String labels_string[9] = {
    "controller.dooropened",
    "controller.setkeyvalid",
    "time",
    "controller.doorclosed",
    "door.unlockdoor",
    "controller.setunarmed",
    "controller.alarma",
    "controller.setarmed",
    "door.lockdoor",
};

int transitions[22][4] = {
    { 0, 0, -1, 9 },
    { 0, 1, -1, 4 },
    { 1, 2, 1, 0 },
    { 2, 3, -1, 6 },
    { 2, 1, -1, 13 },
    { 3, 4, -1, 6 },
    { 4, 5, -1, 3 },
    { 5, 1, -1, 4 },
    { 6, 0, -1, 15 },
    { 6, 1, -1, 10 },
    { 7, 6, -1, 5 },
    { 8, 3, -1, 7 },
    { 8, 1, -1, 12 },
    { 9, 6, -1, 8 },
    { 10, 7, -1, 16 },
    { 11, 3, -1, 6 },
    { 12, 5, -1, 14 },
    { 13, 1, -1, 2 },
    { 14, 4, -1, 2 },
    { 15, 2, 1, 11 },
    { 16, 8, -1, 1 },
};
