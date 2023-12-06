/*
	Code is from Fereidoun Moradis repo cast_function
	https://github.com/fereidoun-moradi/cast_function 

	Changes have been made to make it run as a stand-alone C++-program and
	 making it take in a .statespace-file as argument so it could be scripted.
*/

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string>

int main (int argc, char *argv[]) {
	auto state_timeValue = std::vector<int>();
    auto stateId_msg = std::vector<std::vector<std::string>>(1000000);
    int advancesTime = 0;
    int cntShift = 0;
    auto stateId_varValue = std::vector<std::vector<std::string>>(10000000);
    auto stateId_varName = std::vector<std::vector<std::string>>(10000000);
 	
	auto state_Id = std::vector<std::string>();
	auto stateId = std::vector<std::string>();
	int index = -1;


    // std::cout << "Loading a state transition system (LTS)!" << '\n';
	std::string stateVar = "<variable";
	std::vector<std::string> transitions;
	std::vector<std::string> vars;
	
	std::ifstream readFile;
	// std::ofstream writeFile;
	// std::ofstream writeAutFile;

	std::string filename;
	// std::string outfile;

	if (argc < 2) {
		std::cout << "Usage: cast <statespace>" << std::endl;
		
	} else {
		filename = argv[1];
		// outfile = argv[2];
	}
	
	readFile.open(filename);
	if (!readFile.is_open()) {
		exit(EXIT_FAILURE);
	}
	
	
	std::string annotateVar = "Y";
	std::string ignoreList;
	std::string annotateTrans;
	std::string ignoreListTrans;
	std::string nonDeterVar;
	

	while(!readFile.eof()){

		std::string line;
		
		getline(readFile, line);

		size_t foundID = line.find("id="); 

		size_t sPoint, ePoint;

		sPoint = line.find("id=");
		ePoint = line.find("_");

		size_t foundTransition = line.find("<transition");
		
		if (foundTransition != std::string::npos){

			std::string src = line;
			size_t source = src.find("source");
			size_t endSource = src.find("_");
			std::string srcNumber = src.substr(source+8, endSource-source-8);

			std::string dest = line; 
			size_t destination = dest.find("destination=");
			size_t endDestination = dest.find("executionTime");
			std::string destNumber = dest.substr(destination+13, endDestination-destination-17);

			std::string shift = line;
			size_t shifts = shift.find("shift=");
			size_t shiftd = shift.find(">");
			std::string shiftValue = shift.substr(shifts+7, shiftd-shifts-8);

			if (shifts != std::string::npos ){
				if(shiftValue != "0"){
					cntShift = cntShift +1; 
				}
			}

			if (source != std::string::npos ){
				transitions.push_back(line);
			}
		}

		if (foundID != std::string::npos){

			std::string case_Id = line.substr(sPoint+4, (ePoint-sPoint-4));
			state_Id.push_back(case_Id);
			
			index = index + 1;
			stateId_varValue[index].push_back(case_Id);
			stateId_varName[index].push_back(case_Id);
			stateId_msg[index].push_back(case_Id);
			

			bool finishedState = false;
			while(!finishedState){

				getline(readFile, line);
				size_t foundStateVar = line.find(stateVar);
				size_t foundStateMsg = line.find("<message");
				size_t foundStateVarTime = line.find("</now>");
				size_t foundEndLine = line.find("</state>");					

				if (foundStateVar != std::string::npos){

					size_t startPoint, endPoint;

					startPoint = line.find(">");
					endPoint = line.find("</");

					std::string result = line.substr(startPoint+1, (endPoint-startPoint-1));
					stateId_varValue[index].push_back(result);
					
					size_t startPointName, endPointName;

					startPointName = line.find(".");
					endPointName = line.find(" type");
					std::string resultName = line.substr(startPointName+1, (endPointName-startPointName-2));
					stateId_varName[index].push_back(resultName);
				}
				
				if (foundStateVarTime != std::string::npos){
					size_t sTimePoint, eTimePoint; 
					sTimePoint = line.find(">");
					eTimePoint = line.find("</");
					std::string timeValue = line.substr(sTimePoint+1, (eTimePoint-sTimePoint-1));
					state_timeValue.push_back(stoi(timeValue));
				}
				
				if (foundStateMsg != std::string::npos){
					size_t sTimePoint, eTimePoint; 
					sTimePoint = line.find(">");
					eTimePoint = line.find("</");
					std::string msgName = line.substr(sTimePoint+1, (eTimePoint-sTimePoint-1));
					stateId_msg[index].push_back(msgName);
				}
				
				if (foundEndLine != std::string::npos){
					finishedState = true;
				}
			}
		}
	}

	std::vector<std::vector<std::string> > edges(state_Id.size() + cntShift);
	std::vector<std::vector<std::string> > labels(state_Id.size() + cntShift);

	//find transitions that have same sources
	std::vector<std::string> branch;
	for(int i = 0; i <= index; i++){
		branch.push_back("NO");
	}
	
	for(int i = 0; i < transitions.size(); i++){
		size_t source1 = transitions[i].find("source");
		size_t endSource1 = transitions[i].find("_");
		std::string srcNumber1 = transitions[i].substr(source1+8, endSource1-source1-8);
		
		for(int j = 1; i+j < transitions.size(); j++){
			size_t source2 = transitions[i+j].find("source");
			size_t endSource2 = transitions[i+j].find("_");
			std::string srcNumber2 = transitions[i+j].substr(source2+8, endSource2-source2-8);
			
			if(srcNumber1 == srcNumber2){
				for(int i = 0; i <= index; i++){
					if(stateId_varValue[i][0] == srcNumber1){
						branch[i] = "YES";
					}
				}
			}
		}
	} 
	
	//find transitions that have the same sources and titles (nondeterministic transitions)
	std::vector<std::string> branch_nondeter;
	for(int i = 0; i <= index; i++){
		branch_nondeter.push_back("NO");
	}
	
	for(int i = 0; i < transitions.size(); i++){
		size_t source1 = transitions[i].find("source");
		size_t endSource1 = transitions[i].find("_");
		std::string srcNumber1 = transitions[i].substr(source1+8, endSource1-source1-8);
		
		size_t startTitle = transitions[i].find("title=");
		size_t endTitle = transitions[i].find("/>");
		std::string titleSR1 = transitions[i].substr(startTitle+7, endTitle-startTitle-8);
		
		for(int j = 1; i+j < transitions.size(); j++){
			size_t source2 = transitions[i+j].find("source");
			size_t endSource2 = transitions[i+j].find("_");
			std::string srcNumber2 = transitions[i+j].substr(source2+8, endSource2-source2-8);
			
			size_t startTitle = transitions[i+j].find("title=");
			size_t endTitle = transitions[i+j].find("/>");
			std::string titleSR2 = transitions[i+j].substr(startTitle+7, endTitle-startTitle-8);
			
			if(srcNumber1 == srcNumber2 && titleSR1 == titleSR2){
				for(int i = 0; i <= index; i++){
					if(stateId_varValue[i][0] == srcNumber1){
						branch_nondeter[i] = "YES";
					}
				}
			}
		}
	}
	
	for(int i = 0; i < transitions.size(); i++){

		std::string trans = transitions[i];
		int shiftV = 0;

		size_t source = trans.find("source");
		size_t endSource = trans.find("_");
		std::string srcNumber = trans.substr(source+8, endSource-source-8);

		size_t destination = trans.find("destination=");
		size_t endDestination = trans.find("executionTime");
		std::string destNumber = trans.substr(destination+13, endDestination-destination-17);
		
		size_t destinationExe = trans.find("executionTime=");
		size_t endDestinationExe = trans.find("shift");
		std::string executionTime = trans.substr(destinationExe+15, endDestinationExe-destinationExe-17);

		size_t shifts = trans.find("shift=");
		size_t shiftd = trans.find(">");
		std::string shiftValue = trans.substr(shifts+7, shiftd-shifts-8);
		if(shifts != std::string::npos){
			shiftV = stoi(shiftValue);
		}

		
		std::string tranTime;
		size_t startTimeTrans = trans.find("<time value=");
		size_t endTimeTrans = trans.find("/>");
		if(startTimeTrans != std::string::npos){
			tranTime = trans.substr(startTimeTrans+13, endTimeTrans-startTimeTrans-14);
			advancesTime = stoi(tranTime);
		} 
		
		std::string owner;
		size_t startownTitle = trans.find("owner=");
		size_t endownTitle = trans.find("title");
		if(startownTitle != std::string::npos){
			owner = trans.substr(startownTitle+7, endownTitle-startownTitle-9);
		} 
		
		size_t startTitle = trans.find("title=");
		size_t endTitle = trans.find("/>");
		std::string title = trans.substr(startTitle+7, endTitle-startTitle-8);
		std::string newStateNumber;
		//make the title in transition lowercase 
		for(char &ch : title){
			ch = std::tolower(ch);
		}

		if(annotateVar == "Y"){ 
		if(ignoreListTrans.find(title) == std::string::npos){
		
		//replace title of transition with msg from Q
		for(int i = 0; i <= index; i++){
			if(stateId_varValue[i][0] == srcNumber){
				for(int msg = 1; msg < stateId_msg[i].size(); msg++){
					for(char &ch : stateId_msg[i][msg]){
						ch = std::tolower(ch);
					}
				
					if(stateId_msg[i][msg].find(title) != std::string::npos){
						title = stateId_msg[i][msg];
						break; 
					}
				} 
			}
		}
		
		
		for(int i = 0; i <= index; i++){
			if(stateId_varValue[i][0] == srcNumber){
				title = owner + "." + title + ".[";
				
				for(int m = 0; m <= index; m++){
					if(stateId_varValue[m][0] == destNumber){
						for(int v = 1; v < stateId_varValue[i].size(); v++){
							if(stateId_varValue[i][v] != stateId_varValue[m][v] && branch_nondeter[i]=="YES"){
								//consider ignore list
								if(ignoreList.find(stateId_varName[m][v]) == std::string::npos){
									//put values if transition is not in the igorelistTrans
									title = title + "_" + stateId_varValue[m][v];   
								} 
							} else {
								if(title.find("[0") == std::string::npos){
									
								} else {
									title = title + "0" ;
								}
							}
						} 
					} 
				}
			}
		}
			title = title + "]";
			for(char &ch : title){
					ch = std::tolower(ch);
			}
			
			for (int i = 1; i < title.size(); i++) {
				if (title[i] == '(') {
					title[i] = '[';
				}  
				if (title[i] == ')') {
					title[i] = ']';
				}
			}
		}
	}
		int cnt = state_Id.size();
		for(int j = 0; j < state_Id.size(); j++){
			if(state_Id[j] == srcNumber){
				if(shiftV != 0){
					cnt = cnt +1;
					edges[j].push_back(std::to_string(cnt));
					state_Id.push_back(std::to_string(cnt));
					labels[cnt-1].push_back("@[" + executionTime + ">>" + shiftValue + "]"); //+ title
					edges[cnt-1].push_back(destNumber);
					

				} else{
					edges[j].push_back(destNumber);
				}
				if(startTimeTrans != std::string::npos){ 
					labels[j].push_back("time +=" + tranTime + ""); 
					break;
				} else { 
					labels[j].push_back(title);
					break;
				}
			}
		}
	}
		
	//print out the state transition system with time shift transitions
	// writeFile.open(outfile, std::ofstream::out | std::ofstream::trunc);
	// std::cout << "NUBMER OF STATES: " << state_Id.size() << " NUBMER OF TRANSITIONS: " << transitions.size() +  cntShift <<"\n";
	std::cout << "des (0, " << transitions.size() +  cntShift << ", " << state_Id.size() << ")" <<'\n';
	// writeFile << "des (0, " << transitions.size() +  cntShift << ", " << state_Id.size() << ")" <<'\n';
	for(int i = 0; i < state_Id.size(); i++){
		std::cout << "("<<  stoi(state_Id[i])-1 << ",\"";
		// writeFile << "("<<  stoi(state_Id[i])-1 << ",\"";
		for(int j = 0 ; j < edges[i].size(); j++) {
			if(j>0) {
				std::cout << "("<< stoi(state_Id[i])-1 << ",\"" << labels[i][j] << "\"," <<  stoi(edges[i][j])-1 << ")"<< '\n';
				// writeFile << "("<< stoi(state_Id[i])-1 << ",\"" << labels[i][j] << "\"," <<  stoi(edges[i][j])-1 << ")"<< '\n';
				stateId.push_back(state_Id[i]);
			} else {
				std::cout << labels[i][j] << "\"," <<  stoi(edges[i][j])-1 << ")"<< '\n';
				// writeFile << labels[i][j] << "\"," <<  stoi(edges[i][j])-1 << ")"<< '\n';
				stateId.push_back(state_Id[i]);
			}
		}
	}
}