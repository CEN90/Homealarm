constexpr int total_transitions = 15;
constexpr int total_states = 18;
constexpr int start_state = 6;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setkeyvalid,
    time,
    controller_doorclosed,
    door_unlockdoor,
    controller_setunarmed,
    controller_setarmed,
    controller_alarma,
    door_lockdoor,
};

String labels_string[9] = {
    "controller.dooropened",
    "controller.setkeyvalid",
    "time",
    "controller.doorclosed",
    "door.unlockdoor",
    "controller.setunarmed",
    "controller.setarmed",
    "controller.alarma",
    "door.lockdoor",
};

int transitions[19][4] = {
    { 0, 0, -1, 13 },
    { 0, 1, -1, 4 },
    { 1, 2, 1, 0 },
    { 2, 3, -1, 6 },
    { 2, 1, -1, 7 },
    { 3, 4, -1, 6 },
    { 4, 5, -1, 3 },
    { 5, 1, -1, 10 },
    { 6, 0, -1, 12 },
    { 6, 1, -1, 11 },
    { 7, 1, -1, 2 },
    { 8, 3, -1, 6 },
    { 9, 4, -1, 2 },
    { 10, 5, -1, 9 },
    { 11, 6, -1, 14 },
    { 12, 2, 1, 8 },
    { 13, 7, -1, 5 },
    { 14, 8, -1, 1 },
};
