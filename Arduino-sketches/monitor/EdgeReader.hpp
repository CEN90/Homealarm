#include <SD.h>

#define PIN_SPI_CS 53 // Mega
// #define PIN_SPI_CS 10 // Uno 
#define BAUD       115200

struct Edge {
    int from;
    int label;
    int value;
    int to;
};

typedef struct Edge* Transitions;

class EdgeReader {
    private:
        int parseValue(String line);
        bool checkStorage(String filename);
        int replaceLabel(String label);
        struct Edge edgeStruct(String line);
        int readFile(String filename);
        int readDesc(String line, int* start_state, int* len_state);

    public:
        EdgeReader();
        ~EdgeReader();

        void printEdge(struct Edge edge);
        int findState(int searched, int* startpos, int* endpos);
        int readTransitions(String statetransitions, String* names, int name_len);
        
        String* edgenames;
        struct Edge* edges;
        int len_state = 0;
        int edges_size = 0;
        int start_state = 0;
        int edgenames_size = 0;
};

EdgeReader::EdgeReader() { }

EdgeReader::~EdgeReader() { }

// Workaround for finding all transitions with the same from-state
int EdgeReader::findState(int searched, int* startpos, int* endpos) {
    *startpos = searched;

    while (edges[*startpos].from != searched && *startpos < edges_size) {
        *startpos += 1;
    }

    *endpos = *startpos;
    
    while (edges[*endpos + 1].from == searched && *endpos < edges_size) {
        *endpos += 1;
    }
    
    int len = *endpos - *startpos + 1;
    return len > 0 ? len : -1; // hedge against fuck ups, -1 if error
}

// Read in transitions from sd-card, returns number of lines read
int EdgeReader::readTransitions(String statetransitions, String* names, int name_len) {
    edgenames = names;
    edgenames_size = name_len;

    edges_size = readFile(statetransitions);
    return edges_size;
}

// Storing transition label as a string and ref later due to memory shortage on arduino
int EdgeReader::replaceLabel(String label) {
    int replacement = 0;

    for (int i = 0; i < edgenames_size; i++) {   
        if (label.startsWith(edgenames[i])) {
            replacement = i;
        }
    }

    return replacement; // ID for later
}

// Parses a value from label string, -1 if no value found
int EdgeReader::parseValue(String label) {
    int time = label.indexOf("+="); // "time +=<int>"
    if (time != -1) {
        return label.substring(time + 2, label.indexOf("\"")).toInt();
    }
    
    int param = label.indexOf("["); // controller.setcurrent[<int>]
    if (param != -1) {
        return label.substring(param + 1, label.indexOf("]")).toInt();
    }

    return -1; // error
}

// Given a string containing an edge return a struct Edge with the data
struct Edge EdgeReader::edgeStruct(String line) {
    if (!line.startsWith("(") && !line.endsWith(")")) {
        Serial.println(line);
        Serial.println(F("Mismatched input, not an edge!"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    String sub_str = line.substring(1, line.length() - 1); 
    String label = sub_str.substring(sub_str.indexOf("\"") + 1, sub_str.lastIndexOf("\""));
    
    Edge edge;
    
    edge.from = sub_str.substring(0, sub_str.indexOf(",")).toInt();
    edge.to = sub_str.substring(sub_str.lastIndexOf(",") + 1).toInt();
    edge.label = replaceLabel(label);
    edge.value = parseValue(label);

    return edge;
}

// for testing purposes
void EdgeReader::printEdge(struct Edge edge) {
    Serial.print(edge.from);
    Serial.print(" -> ");
    Serial.print(edgenames[edge.label]);
    Serial.print(" -> ");
    Serial.print(edge.to);
    Serial.print(": ");
    Serial.println(edge.value);
}

// Simple pre-flight checks
bool EdgeReader::checkStorage(String filename) {
    if (!SD.begin(PIN_SPI_CS)) {
        Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
        return false;
    }
    
    if (!SD.exists(filename)) {
        Serial.println(F("File not found"));
        return false;
    }

    return true;
}

// Get total transitions from line 'des ([start], [total transitions], [total states])'
int EdgeReader::readDesc(String line, int* start_state, int* len_state) {
    if (!line.startsWith("des")) {
        return -1;
    }
    
    // save start state in field start_state, needed for monitor later
    *start_state = line.substring(line.indexOf("(") + 1, line.indexOf(",")).toInt();
    *len_state = line.substring(line.lastIndexOf(",") + 1, line.indexOf(")")).toInt();
    
    // return [total transitions]
    return line.substring(line.indexOf(",") + 1, line.lastIndexOf(",")).toInt();
}

// Reads in transitions from filename, returns lines read. Transitions are stored as structs in field edges
int EdgeReader::readFile(String filename) {
    if (!checkStorage(filename)) {
        Serial.println(F("Error opening SD-card"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    File file = SD.open(filename);
    int file_len = readDesc(file.readStringUntil('\n'), &start_state, &len_state);

    if (file_len <= 0) { // Something wrong with file
        Serial.println(F("Error reading number of states"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    edges = new Edge[file_len];

    if (edges == NULL) { // allocation check
        Serial.println(F("Error allocating memory"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    int lines_added = 0;

    // Read file until eof
    while (file.available()) {
        edges[lines_added] = edgeStruct(file.readStringUntil('\n'));
        lines_added += 1;
    }
    
    file.close();

    return lines_added;
}
