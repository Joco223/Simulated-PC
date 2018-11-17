#include "CPU.h"

int main(int argc, char** argv) {

	CPU cpu;

	/*
	Example program which sets register 0 to 5 and
	register 1 to 9, adds up the values and stores
	them in register 0, and then prints the value
	to the console.
	
	cpu.setMemory8 (0x0,    0x1); //Instruction 1
	cpu.setMemory8 (0x1,    0x0); //Argument 1: register 0 (target register)
	cpu.setMemory8 (0x2,    0x5); //Argument 2: value of 5
	cpu.setMemory8 (0x4,    0x1); //Instruction 1
	cpu.setMemory8 (0x5,    0x1); //Argument 1: register 1 (target register)
	cpu.setMemory8 (0x6,    0x9); //Argument 2: value of 9
	cpu.setMemory8 (0x8,    0x6); //Instruction 6
	cpu.setMemory8 (0x9,    0x0); //Argument 1: register 0 (target register)
	cpu.setMemory8 (0xA,    0x0); //Argument 2: register 0 (first register to sum)
	cpu.setMemory8 (0xB,    0x1); //Argument 3: register 1 (second register to sum)
	cpu.setMemory8 (0xC,   0x21); //Instruction 21
	cpu.setMemory16(0xD, 0xFFF0); //Argument 1: register 0 (target register in 0xFFF0..0xFFF7 format)
	*/

	/*
	Example program which takes in a line of input
	and prints the first character of the input to
	the console.

	cpu.setMemory8 (0x0,   0x22); //Instruction 22
	cpu.setMemory16(0x1,  0x100); //Argument 1: memory address 0x100 (target memory address)
	cpu.setMemory8 (0x3,    0x2); //Instruction 2
	cpu.setMemory8 (0x4,    0x0); //Argument 1: register 0 (target register)
	cpu.setMemory16(0x5,  0x100); //Argument 2: memory address 0x100 (target memory address)
	cpu.setMemory8 (0x7,   0x21); //Instruction 21
	cpu.setMemory16(0x8, 0xFFF0); //Argument 1: register 0 (target register in 0xFFF0..0xFFF7 format)
	*/


	/*
	Example program which calls a function with 2 arguments,
	the function mutliplies the arguments and returns the product
	to the designated return register and then print it.

	Flow of the program:
	1. Push argument 1 to the temp stack
	2. Push argument 2 to the temp stack
	3. (Call instruction) Push the return register index to the stack then next instruction memory address to stack
	   and then the previous 2 arguments to the stack (It looks like this: Top is argument 2 -> argument 1 -> next instruction memory location -> return register),
	   then jump to the function memory location
	---(Now in the function)
	4. Pop the top of the stack (argument 2) and store it into register 1
	5. Pop the top of the stack (argument 1) and store it into register 2
	6. Multiply contents of register 1 by contents of register 2 and store into register 3
	7. Jump to the instruction after the call instruction and store contents of register 3
	   into designated register 0
	8. Print the contents of the register 0

	cpu.setMemory8 (0x0 ,   0x1C); //Push variable 1 to the temp stack (Instruction 0x1C)
	cpu.setMemory16(0x1 ,     10); //Set argument 1: value 10
	cpu.setMemory8 (0x3 ,   0x1C); //Push variable 2 to the temp stack (Instruction 0x1C)
	cpu.setMemory16(0x4 ,      5); //Set argument 1: value 5
	cpu.setMemory8 (0x6 ,   0x1E); //Call the function (instruction 0x1E)
	cpu.setMemory8 (0x7 ,      0); //Set argument 1: register 0
	cpu.setMemory16(0x8 ,    0xE); //Jump to function memory location (Instruction 0xE)
	cpu.setMemory8 (0xA ,   0x20); //Print int (Instruction 0x20)
	cpu.setMemory16(0xB , 0xFFF0); //Set argument 1: register 0
	cpu.setMemory8 (0xD ,      0); //Nothing
	cpu.setMemory8 (0xE ,   0x1D); //Pop top of stack and store (Instruction 0x1D)
	cpu.setMemory8 (0xF ,      1); //Set argument 1: register 1
	cpu.setMemory8 (0x10,   0x1D); //Pop top of stack and store (Instruction 0x1D)
	cpu.setMemory8 (0x11,      2); //Set argument 1: register 2
	cpu.setMemory8 (0x12,    0x8); //Multiply (Instruction 0x8)
	cpu.setMemory8 (0x13,      3); //Set argument 1: register 3
	cpu.setMemory8 (0x14,      1); //Set argument 2: register 1
	cpu.setMemory8 (0x15,      2); //Set argument 3: register 2
	cpu.setMemory8 (0x16,   0x1F); //Return (Instruction 0x1F)
	cpu.setMemory16(0x17, 0xFFF3); //Set argument 1: register 3 (register in 0xFFF0.0xFFF7 format)
	*/

	while(!cpu.isHalted()) {
		cpu.tick();
	}

	return 0;
}