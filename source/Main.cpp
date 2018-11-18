#include <fstream>

#include "CPU.h"

int main(int argc, char** argv) {

	CPU cpu;

	if(argv[1] != nullptr) {
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

	//These two example programs here will be made in assembly soon™

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

	cpu.setMemory8 (0x0 ,   0x1E); //Push variable 1 to the temp stack (Instruction 0x1C)
	cpu.setMemory16(0x1 ,     10); //Set argument 1: value 10
	cpu.setMemory8 (0x3 ,   0x1E); //Push variable 2 to the temp stack (Instruction 0x1C)
	cpu.setMemory16(0x4 ,      5); //Set argument 1: value 5
	cpu.setMemory8 (0x6 ,   0x21); //Call the function (instruction 0x1E)
	cpu.setMemory8 (0x7 ,      0); //Set argument 1: register 0
	cpu.setMemory16(0x8 ,    0xE); //Set argument 2: function memory location
	cpu.setMemory8 (0xA ,   0x23); //Print int (Instruction 0x20)
	cpu.setMemory16(0xB , 0xFFF0); //Set argument 1: register 0
	cpu.setMemory8 (0xD ,      0); //Nothing
	cpu.setMemory8 (0xE ,   0x20); //Pop top of stack and store (Instruction 0x1D)
	cpu.setMemory8 (0xF ,      1); //Set argument 1: register 1
	cpu.setMemory8 (0x10,   0x20); //Pop top of stack and store (Instruction 0x1D)
	cpu.setMemory8 (0x11,      2); //Set argument 1: register 2
	cpu.setMemory8 (0x12,    0x8); //Multiply (Instruction 0x8)
	cpu.setMemory8 (0x13,      3); //Set argument 1: register 3
	cpu.setMemory8 (0x14,      1); //Set argument 2: register 1
	cpu.setMemory8 (0x15,      2); //Set argument 3: register 2
	cpu.setMemory8 (0x16,   0x22); //Return (Instruction 0x20)
	cpu.setMemory16(0x17, 0xFFF3); //Set argument 1: register 3 (register in 0xFFF0.0xFFF7 format)
	*/

	/*
	Example program which recursievly calls a factorial function.
	First it pushes first argument to the stack, then calls the function, 
	function proceeds to check n (argument) > 1, if true, it pushes n to stack imidiately, 
	calls itself, with the argument n-1, if false, it returns 1. When it returns the first time,
	it pops the top of the stack and stores it into register 1 and stores the previous n in it
	(Because it was overwritten by the sub function which was previously called), multiplies it
	with the return value of the previous function and returns that. After all of that, it returns
	to the original call and prints the value to the console.

	cpu.setMemory8 (0x0 ,   0x1E); //Push variable 1 to the temp stack (Instruction 0x1C)
	cpu.setMemory16(0x1 ,    0x5); //Set argument 1: value 5
	cpu.setMemory8 (0x3 ,   0x21); //Call the function (Instruction 0x1F)
	cpu.setMemory8 (0x4 ,    0x0); //Set argument 1: register 0 (return register)
	cpu.setMemory16(0x5 ,    0xB); ///Set argument 2: function start memory location
	cpu.setMemory8 (0x7 ,   0x23); //Print int (Instruction 0x21)
	cpu.setMemory16(0x8 , 0xFFF0); //Set argument 1: register 0 (register in 0xFFF0.0xFFF7 format)
	cpu.setMemory8 (0xA ,      0); //Nothing
	cpu.setMemory8 (0xB ,   0x20); //Pop top of stack and store (Instruction 0x1E)
	cpu.setMemory8 (0xC ,    0x1); //Set argument 1: register 1
	cpu.setMemory8 (0xD ,    0x1); //Set register 2 to 1 (Instruction 0x1)
	cpu.setMemory8 (0xE ,    0x2); //Set argument 1: register 2
	cpu.setMemory16(0xF ,    0x1); //Set argument 2: value 1
	cpu.setMemory8 (0x11,   0x17); //Check if register 1 holds greater value than register 2 (Instruction 0x16) (Unsigned)
	cpu.setMemory8 (0x12,    0x3); //Set argument 1: target register 3 (result store location)
	cpu.setMemory8 (0x13,    0x1); //Set argument 2: register 1
	cpu.setMemory8 (0x14,    0x2); //Set argument 3: register 2
	cpu.setMemory8 (0x15,   0x19); //Check if the value in register 3 is 0 (Instruction 0x18)
	cpu.setMemory8 (0x16,    0x3); //Set argument 1: register 3
	cpu.setMemory16(0x17,   0x32); //Set argument 2: skip memory locaiton (Where it returns 1)
	cpu.setMemory8 (0x19,    0x1); //Set register 2 value to register 1 value (Instruction 0x1)
	cpu.setMemory8 (0x1A,    0x2); //Set argument 1: register 2
	cpu.setMemory16(0x1B, 0xFFF1); //Set argument 2: value in register 1 (register in 0xFFF0.0xFFF7 format)
	cpu.setMemory8 (0x1D,    0xD); //Decrement register 2 (Instruction 0xC)
	cpu.setMemory8 (0x1E,    0x2); //Set argument 1: register 2
	cpu.setMemory8 (0x1F,   0x1F); //Push value of register 1 to stack (Instruction 0x1D)
	cpu.setMemory16(0x20, 0xFFF1); //Set argument 1: register 1 (register in 0xFFF0.0xFFF7 format)
	cpu.setMemory8 (0x22,   0x1E); //Push value of register 2 to temp stack (Instruction 0x1C)
	cpu.setMemory16(0x23, 0xFFF2); //Set argument 1: register 2 (register in 0xFFF0.0xFFF7 format)
	cpu.setMemory8 (0x25,   0x21); //Call the function (Instruction 0x1C)
	cpu.setMemory8 (0x26,    0x2); //Set argument 1: register 2 (return register)
	cpu.setMemory16(0x27,    0xB); //Set argument 2: function start memory location
	cpu.setMemory8 (0x29,   0x20); //Pop top of stack and store (Instruction 0x1E)
	cpu.setMemory8 (0x2A,    0x1); //Set argument 1: register 1
	cpu.setMemory8 (0x2B,    0x8); //Multiply register 1 by register 2 (Instruction 0x8)
	cpu.setMemory8 (0x2C,    0x3); //Set argument 1: register 3
	cpu.setMemory8 (0x2D,    0x1); //Set argument 2: register 1
	cpu.setMemory8 (0x2E,    0x2); //Set argument 3: register 2
	cpu.setMemory8 (0x2F,   0x22); //Return (Instruction 0x20)
	cpu.setMemory16(0x30, 0xFFF3); //Set argument 1: register 3 (register in 0xFFF0.0xFFF7 format)
	cpu.setMemory8 (0x32,   0x22); //Return (Instruction 0x20)
	cpu.setMemory16(0x33,    0x1); //Set argument 1: value 1
	*/

	while(!cpu.isHalted()) {
		cpu.tick();
	}

	return 0;
}