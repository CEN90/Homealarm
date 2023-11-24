import re
import os

def parse_aut_file(file_path):
    transitions = []
    start_state = total_states = total_transitions = None

    # Open the file in read mode
    with open(file_path, 'r') as file:
        for line in file:
            # If the line starts with 'des', it contains the start state, total states, and total transitions
            if line.startswith('des'):
                match = re.match(r'des \((\d+),(\d+),(\d+)\)', line.strip())
                if match:
                    start_state = int(match.group(1))
                    total_states = int(match.group(2))
                    total_transitions = int(match.group(3))
            else:
                # If the line doesn't start with 'des', it contains a transition
                match = re.match(r'\((\d+),"([^"]+)\[(.*?)\]\.\[\]",(\d+)\)', line.strip())
                if match:
                    from_state = int(match.group(1))
                    label = match.group(2)
                    value_str = match.group(3)
                    if value_str.isdigit():
                        value = int(value_str)
                    elif value_str.lower() == 'true':
                        value = 1
                    elif value_str.lower() == 'false':
                        value = 0
                    else: # If unexpected then don't handle it, monitor can ignore it
                        value = -1
                    to_state = int(match.group(4))
                    transitions.append((from_state, label, value, to_state))
                else:
                    # First regex won't match against time +=<int>
                    match = re.match(r'\((\d+),"([^"]+) \+=([0-9]+)",(\d+)\)', line.strip())
                    if match:
                        from_state = int(match.group(1))
                        label = match.group(2)
                        value = int(match.group(3))
                        to_state = int(match.group(4))
                        transitions.append((from_state, label, value, to_state))

    return start_state, total_states, total_transitions, transitions

def replace_labels(transitions):
    label_to_int = {}
    new_transitions = []

    for transition in transitions:
        from_state, label, value, to_state = transition

        if label not in label_to_int:
            label_to_int[label] = len(label_to_int) 

        new_transitions.append((from_state, label_to_int[label], value, to_state))

    return list(label_to_int.keys()), new_transitions


def printer(file, start_state, total_states, total_transitions, labels, transitions):
    with open(file, 'w') as f:
        f.write(f'int total_transitions = {total_transitions};\n')
        f.write(f'int total_states = {total_states};\n')
        f.write(f'int start_state = {start_state};\n\n')

        f.write('enum t_labels { From, Label, Value, To };\n\n')

        f.write(f'enum transition_labels ' + '{\n')
        for l in labels:
            l_s = l.replace('.', '_')
            f.write(f'    {l_s},\n')
        f.write('};\n\n')

        f.write(f'String labels_string[{len(labels)}] = ' + '{\n')
        for l in labels:
            f.write(f'    \"{l}\",\n')
        f.write('};\n\n')
        f.write(f'uint8_t transitions[{total_states + 1}][4] = ' + '{\n')
        for t in transitions:
            line = '    { ' + str(t[0]) + ', ' + str(t[1]) + ', ' + str(t[2]) + ', ' + str(t[3]) + ' },\n'
            f.write(line)
        f.write('};\n')


file = './Rebeca/src/alarm_states.aut'
start_state, total_states, total_transitions, transitions = parse_aut_file(file)
print(f'Start state: {start_state}, Total states: {total_states}, Total transitions: {total_transitions}')

labels, new_transitions = replace_labels(transitions)

file_out = os.path.dirname(__file__) +'/states.hpp'
printer(file_out, start_state, total_states, total_transitions, labels, new_transitions)
