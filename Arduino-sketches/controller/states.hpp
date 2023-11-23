int total_transitions = 72;
int total_states = 95;
int start_state = 0;
 
String labels[4] = {
    'controller.setcurrent',
    'time',
    'hc_unit.setheating',
    'hc_unit.setcooling'
};

int transitions[2][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
};