constexpr int total_transitions = 11;
constexpr int total_states = 15;
constexpr int start_state = 9;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_dooropened,
    controller_setunarmed,
    controller_doorclosed,
    controller_alarmaoff,
    door_lockdoor,
    door_unlockdoor,
    controller_alarma,
    controller_setarmed,
};

String labels_string[8] = {
    "controller.dooropened",
    "controller.setunarmed",
    "controller.doorclosed",
    "controller.alarmaoff",
    "door.lockdoor",
    "door.unlockdoor",
    "controller.alarma",
    "controller.setarmed",
};

int transitions[16][4] = {
    { 0, 0, -1, 5 },
    { 0, 1, -1, 4 },
    { 1, 2, -1, 6 },
    { 1, 3, -1, 8 },
    { 2, 4, -1, 0 },
    { 3, 1, -1, 9 },
    { 4, 5, -1, 9 },
    { 5, 6, -1, 1 },
    { 6, 0, -1, 7 },
    { 6, 3, -1, 3 },
    { 7, 2, -1, 6 },
    { 8, 1, -1, 10 },
    { 9, 0, -1, 10 },
    { 9, 7, -1, 2 },
    { 10, 2, -1, 9 },
};
