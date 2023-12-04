#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if(argc != 5) {
        std::cout << "Usage: ./program <input_aut> <output_aut> [-s <observable_states_file> | observable_states]\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string output_filename = argv[2];
    std::string observable_actions;

    // Check if the -s flag is used
    if(std::string(argv[3]) == "-s") {
        std::string observable_states_file = argv[4];
        std::ifstream statesFile(observable_states_file);

        if (!statesFile.is_open()) {
            std::cout << "Failed to open the observable states file.\n";
            return 1;
        }

        std::getline(statesFile, observable_actions);
        statesFile.close();
    } else {
        observable_actions = argv[3];
    }

    std::ifstream readFile(filename);
    std::ofstream writeFile(output_filename);

    if (!readFile.is_open()) {
        std::cout << "Failed to open the input file.\n";
        return 1;
    }

    std::string ignoreList = "";
    std::string line;
    while(getline(readFile, line)){
        size_t startPoint, endPoint;

        startPoint = line.find(",\"");
        endPoint = line.find("\",");

        std::string result = line.substr(startPoint+2,endPoint-startPoint-2);

        std::stringstream ss(observable_actions);
        std::string substr;
        int found = 0;
        while(std::getline(ss, substr, ',')){
            if(result.find(substr) != std::string::npos){
                found = 1;
            }
        }

        if (found == 0){
            if(ignoreList.find(result) == std::string::npos){
                ignoreList.append(result);
                writeFile << result << ",";
            }
        }
    }

    readFile.close();
    writeFile.close();

    return 0;
}