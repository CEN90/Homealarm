int total_transitions = 72;
int total_states = 95;
int start_state = 0;

enum t_labels { From, Label, Value, To };

enum transition_labels {
    controller_setcurrent,
    time,
    hc_unit_setheating,
    hc_unit_setcooling,
};

String labels_string[4] = {
    "controller.setcurrent",
    "time",
    "hc_unit.setheating",
    "hc_unit.setcooling",
};

uint8_t transitions[96][4] = {
    { 0, 0, 21, 2 },
    { 0, 0, 22, 3 },
    { 0, 0, 23, 4 },
    { 1, 1, 2, 5 },
    { 2, 1, 2, 6 },
    { 3, 1, 2, 5 },
    { 4, 1, 2, 7 },
    { 5, 0, 21, 8 },
    { 5, 0, 22, 9 },
    { 5, 0, 23, 10 },
    { 6, 0, 21, 8 },
    { 7, 0, 23, 10 },
    { 8, 1, 2, 11 },
    { 9, 1, 2, 12 },
    { 10, 1, 2, 13 },
    { 11, 0, 21, 14 },
    { 12, 0, 22, 15 },
    { 13, 0, 23, 16 },
    { 14, 1, 1, 17 },
    { 15, 1, 1, 18 },
    { 16, 1, 1, 19 },
    { 17, 1, 1, 20 },
    { 18, 1, 1, 21 },
    { 19, 1, 1, 22 },
    { 20, 0, 21, 23 },
    { 21, 0, 22, 24 },
    { 22, 0, 23, 25 },
    { 23, 1, 2, 26 },
    { 24, 1, 2, 27 },
    { 25, 1, 2, 28 },
    { 26, 0, 21, 29 },
    { 27, 0, 22, 30 },
    { 28, 0, 23, 31 },
    { 29, 1, 2, 32 },
    { 30, 1, 2, 33 },
    { 31, 1, 2, 34 },
    { 32, 0, 21, 35 },
    { 32, 0, 22, 36 },
    { 32, 0, 20, 37 },
    { 33, 0, 21, 2 },
    { 33, 0, 22, 38 },
    { 33, 0, 23, 4 },
    { 34, 0, 22, 36 },
    { 34, 0, 23, 39 },
    { 34, 0, 24, 40 },
    { 35, 1, 2, 41 },
    { 36, 1, 2, 42 },
    { 37, 2, -1, 43 },
    { 38, 1, 2, 5 },
    { 39, 1, 2, 44 },
    { 40, 3, -1, 45 },
    { 41, 0, 21, 8 },
    { 41, 0, 22, 9 },
    { 41, 0, 20, 46 },
    { 42, 0, 22, 9 },
    { 43, 1, 2, 47 },
    { 44, 0, 22, 9 },
    { 44, 0, 23, 10 },
    { 44, 0, 24, 48 },
    { 45, 1, 2, 49 },
    { 46, 2, -1, 50 },
    { 47, 0, 20, 50 },
    { 48, 3, -1, 51 },
    { 49, 0, 24, 51 },
    { 50, 1, 2, 52 },
    { 51, 1, 2, 53 },
    { 52, 0, 20, 54 },
    { 53, 0, 24, 55 },
    { 54, 1, 1, 56 },
    { 55, 1, 1, 57 },
    { 56, 1, 1, 58 },
    { 57, 1, 1, 59 },
    { 58, 0, 20, 60 },
    { 59, 0, 24, 61 },
    { 60, 1, 2, 62 },
    { 61, 1, 2, 63 },
    { 62, 0, 20, 64 },
    { 63, 0, 24, 65 },
    { 64, 1, 2, 66 },
    { 65, 1, 2, 67 },
    { 66, 0, 21, 2 },
    { 66, 0, 22, 36 },
    { 66, 0, 20, 68 },
    { 67, 0, 22, 36 },
    { 67, 0, 23, 4 },
    { 67, 0, 24, 69 },
    { 68, 1, 2, 70 },
    { 69, 1, 2, 71 },
    { 70, 0, 21, 8 },
    { 70, 0, 22, 9 },
    { 70, 0, 20, 50 },
    { 71, 0, 22, 9 },
    { 71, 0, 23, 10 },
    { 71, 0, 24, 51 },
    { 71, 0, 1, 51 },
    { 71, 0, 0, 51 },
};
