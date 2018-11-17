#include "CPU.h"

int main(int argc, char** argv) {

	CPU cpu;

	/*
	Example program which sets register 0 to 5 and
	register 1 to 9, adds up the values and stores
	them in register 0, and then prints the value
	to the console.
	
	cpu.setMemory8(0x0, 0x1); //Instruction 1
	cpu.setMemory8(0x1, 0x0); //Argument 1: register 0 (target register)
	cpu.setMemory8(0x2, 0x5); //Argument 2: value of 5
	cpu.setMemory8(0x4, 0x1); //Instruction 1
	cpu.setMemory8(0x5, 0x1); //Argument 1: register 1 (target register)
	cpu.setMemory8(0x6, 0x9); //Argument 2: value of 9
	cpu.setMemory8(0x8, 0x6); //Instruction 6
	cpu.setMemory8(0x9, 0x0); //Argument 1: register 0 (target register)
	cpu.setMemory8(0xA, 0x0); //Argument 2: register 0 (first register to sum)
	cpu.setMemory8(0xB, 0x1); //Argument 3: register 1 (second register to sum)
	cpu.setMemory8(0xC, 0x21); //Instruction 21
	cpu.setMemory16(0xD, 0xFFF0); //Argument 1: register 0 (target register in 0xFFF0..0xFFF7 format)
	*/

	/*
	Example program which takes in a line of input
	and prints the first character of the input to
	the console.

	cpu.setMemory8(0x0, 0x22); //Instruction 22
	cpu.setMemory16(0x1, 0x100); //Argument 1: memory address 0x100 (target memory address)
	cpu.setMemory8(0x3, 0x2); //Instruction 2
	cpu.setMemory8(0x4, 0x0); //Argument 1: register 0 (target register)
	cpu.setMemory16(0x5, 0x100); //Argument 2: memory address 0x100 (target memory address)
	cpu.setMemory8(0x7, 0x21); //Instruction 21
	cpu.setMemory16(0x8, 0xFFF0); //Argument 1: register 0 (target register in 0xFFF0..0xFFF7 format)

	*/

	while(!cpu.isHalted()) {
		cpu.tick();
	}

	return 0;
}