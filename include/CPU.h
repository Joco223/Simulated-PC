#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Stack.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

class CPU {
private:
	bool halt;
	bool overflow;
	bool underflow;
	bool debug;

	u16 OPcode;
	u16 programCounter;

	jci::Stack<u16> stack;
	std::vector<u16> tempStack;
	std::vector<u16> registers;
	std::vector<u8> memory;

	std::vector<std::string> log;

	void execute();
	u16 decodeParamOut(u16);
	u16 addTwoU8(u8, u8);
public:
	CPU(bool);
	bool isHalted();
	void tick();
	void setMemory8(u16, u8);
	void setMemory16(u16, u16);
};