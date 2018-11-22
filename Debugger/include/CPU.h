#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Stack.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

struct cpuData {
	u16 programCounter;
	u16 value;
	bool halt;
	bool overflow;
	bool underflow;
	std::vector<u16> stack;
	std::vector<u16> tempStack;
	std::vector<u16> registers;
	std::vector<std::pair<u16, int>> memoryChange;
};

class CPU {
private:
	bool halt;
	bool overflow;
	bool underflow;

	u16 OPcode;
	u16 programCounter;

	jci::Stack<u16> stack;
	std::vector<u16> tempStack;
	std::vector<u16> registers;
	std::vector<std::pair<u16, int>> memoryChange;

	void execute();
	u16 decodeParamOut(u16);
	u16 addTwoU8(u8, u8);
public:
	std::vector<u8> memory;
	CPU();
	bool isHalted();
	cpuData tick();
	void tick2();
	void setMemory8(u16, u8);
	void setMemory16(u16, u16);
};