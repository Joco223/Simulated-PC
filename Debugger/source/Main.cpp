#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <sys/ioctl.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>

#include "CPU.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

std::vector<std::string> hexTable = {
 "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "a",  "b",  "c",  "d",  "e",  "f", "10", "11",
"12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
"24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", "30", "31", "32", "33", "34", "35",
"36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
"48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
"5a", "5b", "5c", "5d", "5e", "5f", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
"6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d",
"7e", "7f", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", "a0", "a1",
"a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
"b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", "c0", "c1", "c2", "c3", "c4", "c5",
"c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
"d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
"ea", "eb", "ec", "ed", "ee", "ef", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
"fc", "fd", "fe", "ff"};

std::vector<std::string> instructionTable {
"halt", "setr", "mread8", "mread16", "mwrite8", "mwrite16", "add", "sub", "mlt", "divu", "divs", "mod", "inc",
"dec", "clr", "and", "or", "not", "bsright", "bsleft", "eql", "diff", "lrgs", "lrgu", "jmp", "jmrz", "jmro",
"jmo", "jmu", "rst", "pusht", "pushs", "pops", "call", "ret", "outu", "outs", "outc", "in"
};

std::vector<std::string> truthTable {"False", "True"};


std::string formatCPUData(std::string name, int value, int value2, int size, std::string colour, int instructionMode) {
	std::string data = name;
	if(instructionMode == 1){
		data = data + colour + "0x" + hexTable[value & 0xFF] + hexTable[value >> 8] + "\033[0m";
		int l1 = data.length() - 4 - colour.length();
		for(int i = 0; i < size - l1; ++i) {data = data + " ";}
		data = data + "(" + colour + instructionTable[value2] + "\033[0m" + ")";
		int l2 = data.length() - 8 - 2*colour.length();
		for(int i = 0; i < 35 - l2; ++i) {data = data + " ";}
	}else if(instructionMode == 2) {
		data = data + colour + truthTable[value] + "\033[0m";
		int l2 = data.length() - 4 - colour.length();
		for(int i = 0; i < 35 - l2; ++i) {data = data + " ";}
	}else{
		data = data + colour + "0x" + hexTable[value & 0xFF] + hexTable[value >> 8] + "\033[0m";
		int l1 = data.length() - 4 - colour.length();
		for(int i = 0; i < size - l1; ++i) {data = data + " ";}
		data = data + "(" + colour + std::to_string(value2) + "\033[0m" + ")";
		int l2 = data.length() - 8 - 2*colour.length();
		for(int i = 0; i < 35 - l2; ++i) {data = data + " ";}
	}
	
	return data;
}

int main(int argc, char** argv) {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int consoleXSize = 0;
	int consoleYSize = 0;

	#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
	    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	    consoleXSize = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	    consoleYSize = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	#endif
	#ifdef linux
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		consoleXSize = w.ws_col;
		consoleYSize = w.ws_row;
	#endif

	if(consoleYSize < 23) {
		std::cerr << "Console Y size to small. 23 is the smallest possible Y size (characters).\n";
		return 0;
	}
	if(consoleXSize < 104) {
		std::cerr << "Console X size to small. 104 is the smallest possible X size (characters).\n";
		return 0;
	}
	
	std::vector<cpuData> runData;

	CPU cpu;

	if(argc == 1) {
		std::cerr << "\033[31mError\033[0m, no input program.\n";
		return 0;
	}else{
		std::ifstream inputFile(argv[1], std::ios::in | std::ios::binary);
		inputFile.seekg(0, std::ios::end);
		int length = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);

		char* buffer = new char[length];

		inputFile.read(buffer, length);

		for(int i = 0; i < length; ++i) {
			cpu.setMemory8(i, buffer[i]);
		}
		delete buffer;
	}

	std::vector<u8> cpuMemory = cpu.memory;

	std::cerr << "Simulating the program...\n";

	while(!cpu.isHalted()) {
		//cpu.tick2();
		runData.push_back(cpu.tick());
	}

	int offset = 0;

	std::vector<std::vector<std::string>> CPUInfo;

	std::cerr << "Finished the simulation. Formatting the data...\n";

	for(int i = 0; i < runData.size(); ++i) {
		std::vector<std::string> tmpCPUInfo;
		//PPCP
		if(i > 0) {tmpCPUInfo.push_back(formatCPUData("Prev PC Position: ", runData[i-1].programCounter, cpuMemory[runData[i-1].programCounter],  25, "\033[32m", 1));}else{tmpCPUInfo.push_back("Prev PC Position:                  ");}
		//PC
		tmpCPUInfo.push_back(formatCPUData("Program Counter : ", runData[i].programCounter, cpuMemory[runData[i].programCounter], 25, "\033[32m", 1));
		//NPCP
		if(i < runData.size()) {tmpCPUInfo.push_back(formatCPUData("Next PC Position: ", runData[i+1].programCounter, cpuMemory[runData[i+1].programCounter], 25, "\033[32m", 1));}else{tmpCPUInfo.push_back("Next PC Position:                  ");}
		//MAV
		tmpCPUInfo.push_back(formatCPUData("Mem address value: ", cpuMemory[runData[i].programCounter], cpuMemory[runData[i].programCounter], 26, "\033[32m", 0));
		//Arg1
		tmpCPUInfo.push_back(formatCPUData("Argument 1: ", cpuMemory[runData[i].programCounter + 1], cpuMemory[runData[i].programCounter + 1], 19, "\033[32m", 0));
		//Arg2
		tmpCPUInfo.push_back(formatCPUData("Argument 2: ", cpuMemory[runData[i].programCounter + 2], cpuMemory[runData[i].programCounter + 2], 19, "\033[32m", 0));
		//Arg3
		tmpCPUInfo.push_back(formatCPUData("Argument 3: ", cpuMemory[runData[i].programCounter + 3], cpuMemory[runData[i].programCounter + 3], 19, "\033[32m", 0));
		//Divider
		tmpCPUInfo.push_back("-----------------------------------");
		//RA
		tmpCPUInfo.push_back(formatCPUData("Register A: ", runData[i].registers[0], runData[i].registers[0], 19, "\033[96m", 0));
		//RB
		tmpCPUInfo.push_back(formatCPUData("Register B: ", runData[i].registers[1], runData[i].registers[1], 19, "\033[96m", 0));
		//RC
		tmpCPUInfo.push_back(formatCPUData("Register C: ", runData[i].registers[2], runData[i].registers[2], 19, "\033[96m", 0));
		//RD
		tmpCPUInfo.push_back(formatCPUData("Register D: ", runData[i].registers[3], runData[i].registers[3], 19, "\033[96m", 0));
		//RE
		tmpCPUInfo.push_back(formatCPUData("Register E: ", runData[i].registers[4], runData[i].registers[4], 19, "\033[96m", 0));
		//RF
		tmpCPUInfo.push_back(formatCPUData("Register F: ", runData[i].registers[5], runData[i].registers[5], 19, "\033[96m", 0));
		//RG
		tmpCPUInfo.push_back(formatCPUData("Register G: ", runData[i].registers[6], runData[i].registers[6], 19, "\033[96m", 0));
		//RH
		tmpCPUInfo.push_back(formatCPUData("Register H: ", runData[i].registers[7], runData[i].registers[7], 19, "\033[96m", 0));
		//Overflow
		tmpCPUInfo.push_back(formatCPUData("Overflow flag: ", runData[i].overflow, 0, 19, "\033[95m", 2));
		//Undeflow
		tmpCPUInfo.push_back(formatCPUData("Undeflow flag: ", runData[i].underflow, 0, 19, "\033[95m", 2));
		//Halt
		tmpCPUInfo.push_back(formatCPUData("Halt flag: ", runData[i].halt, 0, 19, "\033[95m", 2));
		CPUInfo.push_back(tmpCPUInfo);
	}

	std::cerr << "Finished formatting the data.\n";

	bool quit = false;

	int memOffset = 0;

	while(!quit) {
		std::cerr << "|PC|Memory Address|     Value     |             CPU Info              | Stack (V Top) | TStack (V Top) |\n";
		std::cerr << "|--+--------------+---------------+-----------------------------------+---------------+----------------|\n";

		

		for(int i = 0; i < consoleYSize - 4; ++i) {
			std::cerr << "|";
			int memPos = i + (memOffset * (consoleYSize - 4));
			//Program counter arrow
			if(memPos == runData[offset].programCounter) {std::cerr << "\033[31m=>\033[0m|";}else{std::cerr << "  |";}
			std::stringstream sstream;		
			
			//Memory address
			sstream << "\033[93m" <<"0x" << std::hex << memPos << "\033[0m";
			std::string memoryAddress = sstream.str();
			sstream.clear();
			std::cerr << memoryAddress;
			for(int j = 0; j < 14-(memoryAddress.length() - 9); ++j) {std::cerr << " ";}
			std::cerr << "|";

			//Value
			std::string color1 = "\033[32m";
			std::string value = color1 + "0x" + hexTable[cpuMemory[memPos]] + "\033[0m";
			std::cerr << value;
			for(int j = 0; j < 7-(value.length() - 9); ++j) {std::cerr << " ";}
			std::string valueDec = "(" + color1 + std::to_string(cpuMemory[memPos]) + "\033[0m" + ")";
			std::cerr << valueDec;
			for(int j = 0; j < 8-(valueDec.length() - 9); ++j) {std::cerr << " ";}
			std::cerr << "|";

			memPos++;

			//CPU Info
			if(i < CPUInfo[offset].size()) {
				std::cerr << (CPUInfo[offset])[i];
			}else{
				for(int j = 0; j < 35; ++j) {std::cerr << " ";}
			}
			std::cerr << "|";

			//Stack
			if(i < runData[offset].stack.size()) {
				std::string color1 = "\033[32m";
				std::string value = color1 + "0x" + hexTable[runData[offset].stack[i] & 0xFF] + hexTable[runData[offset].stack[i] >> 8] + "\033[0m";
				std::cerr << value;
				for(int j = 0; j < 7-(value.length() - 9); ++j) {std::cerr << " ";}
				std::string valueDec = "(" + color1 + std::to_string(runData[offset].stack[i]) + "\033[0m" + ")";
				std::cerr << valueDec;
				for(int j = 0; j < 8-(valueDec.length() - 9); ++j) {std::cerr << " ";}
			}else{
				for(int j = 0; j < 15; ++j) {std::cerr << " ";}
			}
			std::cerr << "|";

			//TStack
			if(i < runData[offset].tempStack.size()) {
				std::string color1 = "\033[32m";
				std::string value = color1 + "0x" + hexTable[runData[offset].tempStack[i] & 0xFF] + hexTable[runData[offset].tempStack[i] >> 8] + "\033[0m";
				std::cerr << value;
				for(int j = 0; j < 7-(value.length() - 9); ++j) {std::cerr << " ";}
				std::string valueDec = "(" + color1 + std::to_string(runData[offset].tempStack[i]) + "\033[0m" + ")";
				std::cerr << valueDec;
				for(int j = 0; j < 9-(valueDec.length() - 9); ++j) {std::cerr << " ";}
			}else{
				for(int j = 0; j < 16; ++j) {std::cerr << " ";}
			}
			std::cerr << "|\n";
		}

		std::cerr << "|--+--------------+---------------+-----------------------------------+---------------+----------------|\n";
		std::cerr << "|  S - Prev Inst / W - Next Inst  |   E - Mem PgUp / D - Mem PgDown   |            Q - Exit            |";

		inputLabel:
		char input = _getch();
		if(input == 'q') {
			quit = true;
		}else if(input == 'w') {
			if(offset + 1 < runData.size()) {
				offset++;
				for(int i = 0; i < runData[offset].memoryChange.size(); ++i) {cpuMemory[runData[offset].memoryChange[i].first] = runData[offset].memoryChange[i].second;}
			}else{
				goto inputLabel;
			}
			std::cerr << '\n';
		}else if(input == 's') {
			if(offset - 1 >= 0) {
				offset--;
				for(int i = 0; i < runData[offset].memoryChange.size(); ++i) {cpuMemory[runData[offset].memoryChange[i].first] = runData[offset].memoryChange[i].second;}
			}else{
				goto inputLabel;
			}
			std::cerr << '\n';
		}else if(input == 'e') {
			if((memOffset + 1) * (consoleYSize - 4) < 65520) {
				memOffset++;
			}else{
				goto inputLabel;
			}
			std::cerr << '\n';
		}else if(input == 'd') {
			if((memOffset - 1) * (consoleYSize - 4) >= 0) {
				memOffset--;
			}else{
				goto inputLabel;
			}
			std::cerr << '\n';
		}else{
			goto inputLabel;
		}
	}
	
    return 0;
}