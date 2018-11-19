#include <fstream>

#include "CPU.h"

int main(int argc, char** argv) {

	if(argv[1] != nullptr) {
		std::string debugMode = argv[1];
		if(debugMode != "true" && debugMode != "false") {
			std::cerr << "Debug mode not specificed, put true/false after Simulated-PC.exe\n";
			return 0;
		}
	}else{
		std::cerr << "Debug mode not specificed, put true/false after Simulated-PC.exe\n";
		return 0;
	}
	
	CPU cpu(argv[1]);

	if(argv[2] != nullptr) {
		std::ifstream inputFile(argv[2], std::ios::in | std::ios::binary);
		inputFile.seekg(0, std::ios::end);
		int length = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);

		char* buffer = new char[length];

		inputFile.read(buffer, length);

		for(int i = 0; i < length; ++i) {
			cpu.setMemory8(i, buffer[i]);
		}
		delete buffer;
	}else{
		std::cerr << "You can load an assembled program if you put its name as the second argument.\n";
	}
	
	while(!cpu.isHalted()) {
		cpu.tick();
	}

	return 0;
}