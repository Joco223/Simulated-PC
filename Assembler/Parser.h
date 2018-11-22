#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace Parser{
	struct instructionLine {
		std::string instruction;
		std::vector<std::string> arguments;
		int lineNum;
	};

	std::vector<std::string> parseFile(const char* file) {
		std::ifstream in(file);
		std::string line = "";
		std::vector<std::string> lines;
		while(std::getline(in, line)) {
			if(line[0] != ';' && line != "") {
				lines.push_back(line);
			}
		}
		return lines;
	}

	instructionLine parseLine(std::string& line, int j) {
		std::vector<std::string> arguments;
		std::string instruction = "";
		int i = 0;
		while(line.at(i) != ' ') {
			instruction = instruction + line[i];
			i++;
		}
		std::string argumentsS = line.substr(i);
		std::string newString = "";

		unsigned hasComment = argumentsS.find(';');
		if(hasComment == std::string::npos) {
			newString = argumentsS;
		}else{
			newString = argumentsS.substr(1, hasComment-1);
		}

		std::stringstream ss(newString);
		while(ss.good()) {
			std::string substring;
			std::getline(ss, substring, ',');
			substring.erase(std::remove(substring.begin(), substring.end(), ' '), substring.end());
			arguments.push_back(substring);
		}
		return {instruction, arguments, j};
	}

	std::vector<instructionLine> parseLines(std::vector<std::string>& lines) {
		std::vector<instructionLine> instructionLines;
		int i = 1;
		for(auto& line : lines) {
			instructionLine currentInstruction;
			if(line != "halt" && line != "rst") {
				currentInstruction = parseLine(line, i);
			}else{
				currentInstruction = {"halt", {}, i};
			}	
			instructionLines.push_back(currentInstruction);
			i++;
		}

		return instructionLines;
	}
}

