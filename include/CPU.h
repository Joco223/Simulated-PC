#pragma once

#include <iostream>
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

	u16 OPcode;
	u16 programCounter;

	jci::Stack<u16> stack;
	std::vector<u16> registers;
	std::vector<u8> memory;

	void execute();
	u16 decodeParamOut(u16);
	u16 addTwoU8(u8, u8);
public:
	CPU();
	bool isHalted();
	void tick();
	void setMemory8(u16, u8);
	void setMemory16(u16, u16);
};