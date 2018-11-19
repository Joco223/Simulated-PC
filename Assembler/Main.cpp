#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <locale>

#include "Parser.h"

typedef unsigned char u8;
typedef unsigned short u16;

struct instructionTemplate {
	u16 instructionCode;
	std::vector<u8> argumentSizes;
};

struct label {
	std::string name;
	u16 position;
};

struct instructionToModify {
	Parser::instructionLine IL;
	u16 position;
	std::string name;
	u8 argumentNum;
};

std::unordered_map<std::string, instructionTemplate> instructionSet = {{{"setr"},     {0x1 , {1, 2   }}}, {{"mread8"},  {0x2 , {1, 2   }}},
																	   {{"mread16"},  {0x3 , {1, 2   }}}, {{"mwrite8"}, {0x4 , {1, 2   }}},
																	   {{"mwrite16"}, {0x5 , {1, 2   }}}, {{"add"},     {0x6 , {1, 1, 1}}},
																	   {{"sub"},      {0x7 , {1, 1, 1}}}, {{"mlt"},     {0x8 , {1, 1, 1}}},
																	   {{"divu"},     {0x9 , {1, 1, 1}}}, {{"divs"},    {0xA , {1, 1, 1}}},
																	   {{"mod"},      {0xB , {1, 1, 1}}}, {{"inc"},     {0xC , {1      }}},
																	   {{"dec"},      {0xD , {1      }}}, {{"clr"},     {0xE , {1      }}},
																	   {{"and"},      {0xF , {1, 1, 1}}}, {{"or"},      {0x10, {1, 1, 1}}},
																	   {{"not"},      {0x11, {1, 1,  }}}, {{"bsright"}, {0x12, {1, 1, 1}}},
																	   {{"bsleft"},   {0x13, {1, 1, 1}}}, {{"eql"},     {0x14, {1, 1, 1}}},
																	   {{"diff"},     {0x15, {1, 1, 1}}}, {{"lrgs"},    {0x16, {1, 1, 1}}},
																	   {{"lrgu"},     {0x17, {1, 1, 1}}}, {{"jmp"},     {0x18, {2      }}},
																	   {{"jmrz"},     {0x19, {1, 2   }}}, {{"jmro"},    {0x1A, {1, 2   }}},
																	   {{"jmo"},      {0x1B, {2      }}}, {{"jmu"},     {0x1C, {2      }}},
																	   {{"rst"},      {0x1D, {       }}}, {{"pusht"},   {0x1E, {2      }}},
																	   {{"pushs"},    {0x1F, {2      }}}, {{"pops"},    {0x20, {1      }}},
																	   {{"call"},     {0x21, {1, 2   }}}, {{"ret"},     {0x22, {2      }}},
																	   {{"outu"},     {0x23, {2      }}}, {{"outs"},    {0x24, {2      }}},
																	   {{"outc"},     {0x25, {2      }}}, {{"in"},      {0x26, {2      }}},
																	   {{"halt"},     {0x0 , {       }}}, {{"label"},   {0   , {2      }}}
																	};

std::unordered_map<std::string, u16> registers8  = {{"RA",      0}, {"RB",      1}, {"RC",      2}, {"RD",      3}, {"RE",      4}, {"RF",      5}, {"RG", 6}, {"RH",      7}};
std::unordered_map<std::string, u16> registers16 = {{"RA", 0xFFF0}, {"RB", 0xFFF1}, {"RC", 0xFFF2}, {"RD", 0xFFF3}, {"RE", 0xFFF4}, {"RF", 0xFFF5}, {"RG", 6}, {"RH", 0xFFF7}};

bool isDigit(std::string input) {
	bool isX = false;
	bool hasBeenX = false;
	bool isNonDec = false;
	for(int i = 0; i < input.length(); ++i) {
		if(i == 0) {
			if(input[i] == '0') {
				isX = true;
			}else if(input[i] != '+' || input[i] != '-') {
				if(!isdigit(input[i])) {
					return false;
				}
			}
		}else{
			if(isX) {
				if(input[i] != 'x' && input[i] != 'b') {
					return false;
				}else{
					isX = false;
					hasBeenX = true;
					isNonDec = true;
				}
			}else{
				if(!isNonDec) {
					if(!isdigit(input[i])) {
						return false;
					}
				}else{
					hasBeenX = false;
					std::string usableCharacters = "0123456789abcdefABCDEF";
					bool correct = false;
					for(int j = 0; j < usableCharacters.length(); ++j) {
						if(input[i] == usableCharacters[j]) {correct = true; break;}
					}
					if(!correct) {return false;}
				}
			}
		}
	}
	if(hasBeenX){
		return false;
	}else{
		return true;
	}
}

bool writeInstructions(std::vector<Parser::instructionLine>& instructionLines, const char* fileName, std::string& finalOutputName) {
	std::vector<u8> byteCode;
	std::vector<label> labels;
	std::vector<instructionToModify> instructionsToModify;

	//Go trough all instructions
	for(auto& IL: instructionLines) {
		//Check if instruction exists in the instruction set
		if(instructionSet.find(IL.instruction) != instructionSet.end()) {
			instructionTemplate currentInstruction = instructionSet[IL.instruction];

			if(IL.instruction != "label") {byteCode.push_back(currentInstruction.instructionCode);}

			//Check if instruction has enough arguments
			if(currentInstruction.argumentSizes.size() > 0) {
				//Go trough all the arguments
				for(int i = 0; i < currentInstruction.argumentSizes.size(); i++) {
					if(i <= IL.arguments.size()-1)  {
						if(currentInstruction.argumentSizes[i] == 1) {
							if(registers8.find(IL.arguments[i]) != registers8.end()) {
								byteCode.push_back(registers8[IL.arguments[i]]);
							}else{
								std::cerr << "\033[31mError\033[0m, unknown 8 bit argument: " << IL.arguments[i] << "\033[0m (Argument position: " << i+1 << ") at instruction: " << IL.instruction << " at line: " << IL.lineNum << ".\n";
								std::cerr << "Aborting assembly.\n"; return 0;
							}
						}else{
							if(registers16.find(IL.arguments[i]) != registers16.end()) {
								byteCode.push_back(registers16[IL.arguments[i]] & 0xFF);
								byteCode.push_back(registers16[IL.arguments[i]] >> 8);
							}else{
								if(isDigit(IL.arguments[i])) {
									byteCode.push_back(std::stoi(IL.arguments[i], nullptr, 0) & 0xFF);
									byteCode.push_back(std::stoi(IL.arguments[i], nullptr, 0) >> 8);
								}else{
									if(IL.instruction == "label") {
										labels.push_back({IL.arguments[i], byteCode.size()});
									}else{
										byteCode.push_back(0x0);
										byteCode.push_back(0x0);
										instructionsToModify.push_back({IL, byteCode.size()-2, IL.arguments[i], i});
									}	
								}
							}
						}
					}else{
						std::cerr << "\033[31mError\033[0m, not enough arguments at instruction: " << IL.instruction << " at line: " << IL.lineNum << ", " << currentInstruction.argumentSizes.size() << " number of arguments required.\n";
						std::cerr << "Aborting assembly.\n"; return 0;
					}
				}
			}
		}else{
			std::cerr << "\033[31mError\033[0m, unknown instruction: " << IL.instruction << " found at line: " << IL.lineNum << ".\n";
			std::cerr << "Aborting assembly.\n"; return 0;
		}
	}

	for(int i = 0; i < instructionsToModify.size(); ++i) {
		int id = -1;
		for(int j = 0; j < labels.size(); ++j) {
			if(instructionsToModify[i].name == labels[j].name) {id = j; break;}
		}
		if(id == -1) {
			std::cerr << "\033[31mError\033[0m, unknown argument: \033[35m" << instructionsToModify[i].name << "\033[0m (Argument position: \033[35m" << instructionsToModify[i].argumentNum+1 << "\033[0m) at instruction: \033[35m" << instructionsToModify[i].IL.instruction << "\033[0m, at line: \033[35m" << instructionsToModify[i].IL.lineNum << "\033[0m.\n";
			std::cerr << "Aborting assembly.\n"; return 0;
		}else{
			byteCode[instructionsToModify[i].position    ] = labels[id].position &  0xFF;
			byteCode[instructionsToModify[i].position + 1] = labels[id].position >> 8;
		}	
	}
	
	std::string finalFileName = fileName;
	if(finalFileName.length() < 4) {
		finalFileName = finalFileName + ".cal";
	}else{
		if(finalFileName.substr(finalFileName.length()-4) != ".cal") {
			finalFileName = finalFileName + ".cal";
		}
	}
	finalOutputName = finalFileName;
	std::ofstream outputFile(finalFileName, std::ios::out | std::ios::binary);
	outputFile.write(reinterpret_cast<char*>(byteCode.data()), byteCode.size());
	outputFile.close();
	return 1;
}

int main(int argc, char** argv) {
	if(argc == 1) {std::cerr << "\033[31mError\033[0m, no input file name and no output file name.\n"; return 0;}
	if(argc == 2) {std::cerr << "\033[31mError\033[0m, no input file name/no output file name.\n"; return 0;}

	std::vector<std::string> lines = Parser::parseFile(argv[1]);
	std::vector<Parser::instructionLine> instructionLines = Parser::parseLines(lines);
	std::string output;
	int end = writeInstructions(instructionLines, argv[2], output);
	if(end) {
		std::cerr << "\033[32mAssembly succesful!\033[0m Output file is: \033[35m" + output << "\033[0m";
	}
}