#include "CPU.h"

CPU::CPU() 
	:
	halt(false),
	overflow(false),
	underflow(false),
	OPcode(0),
	programCounter(0)
{
	memory.resize(0xFFF0); //65,520 bytes of ram
	registers.resize(8); //8 general purpose registers
	stack.setMaxLength(255); //255 element long stack
}

bool CPU::isHalted() {return halt;}

//Sets memory at address position to a given value
void CPU::setMemory8(u16 address, u8 value) {
	memory[address] = value;
}

//Sets 2 bytes of memory at address and the next addres to a given value
void CPU::setMemory16(u16 address, u16 value) {
	memory[address] = value & 0xFF;
	memory[address + 1] = value >> 8;
}

//Returns direct value or value in a register
u16 CPU::decodeParamOut(u16 address) {
	if(address < 0xFFF0) {
		return address;
	}else{
		return registers[address - 0xFFF0];
	}
}

//Turns two 8 bit values into a single 16 bit value
u16 CPU::addTwoU8(u8 first, u8 second) {
	return second << 8 | first & 0xFF;
}

//Execute the next instruction which is loded in the OPcode register
void CPU::execute() {
	u8 instruction = OPcode;
	u8 arg1 = memory[programCounter + 1];
	u8 arg2 = memory[programCounter + 2];
	u8 arg3 = memory[programCounter + 3];
	switch(instruction) {
		case 0x0: //Halt
			//std::cerr << "The CPU has been halted.\n";
			halt = true;
			break;

		//Memory management instructions-------------------------------------------------------------------------------------------------------------
		case 0x1: //Set A to B
			registers[arg1] = decodeParamOut(addTwoU8(arg2, arg3));
			programCounter += 4;
			break;
		case 0x2: //Read 1 byte from memory
			registers[arg1] = memory[decodeParamOut(addTwoU8(arg2, arg3))];
			programCounter += 4;
			break;
		case 0x3: //Read 2 bytes from memory
			registers[arg1] = addTwoU8(memory[decodeParamOut(addTwoU8(arg2, arg3))], memory[decodeParamOut(addTwoU8(arg2, arg3)) + 1]);
			programCounter += 4;
			break;
		case 0x4: //Write 1 byte to memory
			memoryChange.push_back({decodeParamOut(addTwoU8(arg2, arg3)), (registers[arg1] & 0xFF) - memory[decodeParamOut(addTwoU8(arg2, arg3))]});
			memory[decodeParamOut(addTwoU8(arg2, arg3))] = registers[arg1] & 0xFF;
			programCounter += 4;
			break;
		case 0x5: //Write 2 bytes to memory
			memoryChange.push_back({decodeParamOut(addTwoU8(arg2, arg3)), (registers[arg1] & 0xFF) - memory[decodeParamOut(addTwoU8(arg2, arg3))]});
			memoryChange.push_back({decodeParamOut(addTwoU8(arg2, arg3) + 1), (registers[arg1] >> 8) - memory[decodeParamOut(addTwoU8(arg2, arg3) + 1)]});
			memory[decodeParamOut(addTwoU8(arg2, arg3))] = registers[arg1] & 0xFF;
			memory[decodeParamOut(addTwoU8(arg2, arg3)) + 1] = registers[arg1] >> 8;
			programCounter += 4;
			break;

		//Arithmetic instructions--------------------------------------------------------------------------------------------------------------------
		case 0x6: //Addition
			if(registers[arg2] + registers[arg3] > 65535) {overflow = true;}
			registers[arg1] = registers[arg2] + registers[arg3];
			programCounter += 4;
			break;
		case 0x7: //Subtraction
			if(registers[arg2] - registers[arg3] < 0) {underflow = true;}
			registers[arg1] = registers[arg2] - registers[arg3];
			programCounter += 4;
			break;
		case 0x8: //Multiplication
			if(registers[arg2] * registers[arg3] > 65535) {overflow = true;}
			registers[arg1] = registers[arg2] * registers[arg3];
			programCounter += 4;
			break;
		case 0x9: //Intiger divison (Unsigned)
			if(registers[arg3] == 0) {std::cerr << "Error, can't divide by zero. Error memory location: " << programCounter << '\n'; halt = true; break;}
			registers[arg1] = registers[arg2] / registers[arg3];
			programCounter += 4;
			break;
		case 0xA: //Intiger divison (Signed)
			if(registers[arg3] == 0) {std::cerr << "Error, can't divide by zero. Error memory location: " << programCounter << '\n'; halt = true; break;}
			registers[arg1] = static_cast<s16>(registers[arg2]) / static_cast<s16>(registers[arg3]);
			programCounter += 4;
			break;
		case 0xB: //Modulo
			registers[arg1] = registers[arg2] % registers[arg3];
			programCounter += 4;
			break;
		case 0xC: //Increment the value in a register
			if(registers[arg1] + 1 > 65535) {overflow = true;}
			registers[arg1]++;
			programCounter += 2;
			break;
		case 0xD: //Decrement the value in a register
			if(registers[arg1] - 1 < 0) {underflow = true;}
			registers[arg1]--;
			programCounter += 2;
			break;
		case 0xE: //Clear a register
			registers[arg1] = 0;
			programCounter += 2;
			break;

		//Bitwise instructions-----------------------------------------------------------------------------------------------------------------------
		case 0xF: //Bitwise and
			registers[arg1] = registers[arg2] & registers[arg3];
			programCounter += 4;
			break;
		case 0x10: //Bitwise or
			registers[arg1] = registers[arg2] | registers[arg3];
			programCounter += 4;
			break;
		case 0x11: //Bitwise not
			registers[arg1] = registers[arg2] ^ 1;
			programCounter += 3;
			break;
		case 0x12: //Bitwise shift right
			registers[arg1] = registers[arg2] >> registers[arg3];
			programCounter += 4;
			break;
		case 0x13: //Bitwise shift left
			registers[arg1] = registers[arg2] << registers[arg3];
			programCounter += 4;
			break;

		//Logical instructions-----------------------------------------------------------------------------------------------------------------------
		case 0x14: //A == B
			registers[arg1] = registers[arg2] == registers[arg3];
			programCounter += 4;
			break;
		case 0x15: //A != B
			registers[arg1] = registers[arg2] != registers[arg3];
			programCounter += 4;
			break;
		case 0x16: //Signed A > B
			registers[arg1] = static_cast<s16>(registers[arg2]) > static_cast<s16>(registers[arg3]);
			programCounter += 4;
			break;
		case 0x17: //Unsigned A > B
			registers[arg1] = registers[arg2] > registers[arg3];
			programCounter += 4;
			break;

		//Flow control instructions------------------------------------------------------------------------------------------------------------------
		case 0x18: //Jump
			programCounter = decodeParamOut(addTwoU8(arg1, arg2));
			break;
		case 0x19: //Jump if the value in a register is equal to 0
			if(registers[arg1] == 0) {
				programCounter = decodeParamOut(addTwoU8(arg2, arg3));
			}else{
				programCounter += 4;
			}
			break;
		case 0x1A: //Jump if the value in a register is equal to 1
			if(registers[arg1] == 1) {
				programCounter = decodeParamOut(addTwoU8(arg2, arg3));
			}else{
				programCounter += 4;
			}
			break;
		case 0x1B: //Jump if overflow flag is on
			if(overflow) {
				programCounter = decodeParamOut(addTwoU8(arg1, arg2));
				overflow = false;
			}else{
				programCounter += 3;
			}
			break;
		case 0x1C: //Jump if udnerflowS flag is on
			if(underflow) {
				programCounter = decodeParamOut(addTwoU8(arg1, arg2));
				underflow = false;
			}else{
				programCounter += 3;
			}
			break;
		case 0x1D: //Reset the flags
			overflow = false;
			underflow = false;
			programCounter++;
			break;

		//Stack operation instructions---------------------------------------------------------------------------------------------------------------
		case 0x1E: //Push contents of A to the temp stack
			tempStack.push_back(decodeParamOut(addTwoU8(arg1, arg2)));
			programCounter += 3;
			break;
		case 0x1F: //Push contents of A to the stack
			stack.push(decodeParamOut(addTwoU8(arg1, arg2)));
			programCounter += 3;
			break;
		case 0x20: //Pop top of the stack and store it in a register
			if(!stack.isEmpty()) {
				registers[arg1] = stack.peek();
				stack.pop();
				programCounter += 2;
			}else{
				std::cerr << "Error, can't pop from stack when the stack is empty. Error memory location: " << programCounter << '\n';
				halt = true;
			}
			break; 
		case 0x21: //Push next instruction memory location to the stack and jump to A
			if(stack.getLength() + tempStack.size() + 1 <= 255) {
				stack.push(arg1);
				stack.push(programCounter + 4);
				for(u16 i = 0; i < tempStack.size(); ++i) {stack.push(tempStack[i]);}
				tempStack.clear();
				programCounter = decodeParamOut(addTwoU8(arg2, arg3));
			}else{
				std::cerr << "Error, can't push new element to the stack when stack is full. Error memory location: " << programCounter << '\n';
				halt = true;
			}
			break;
		case 0x22: //Pop top of the stack and jump to it
			if(!stack.isEmpty()) {
				programCounter = stack.peek();
				stack.pop();	
				registers[stack.peek()] = decodeParamOut(addTwoU8(arg1, arg2));
				stack.pop();
			}else{
				std::cerr << "Error, can't pop from stack when the stack is empty. Error memory location: " << programCounter << '\n';
				halt = true;
			}
			break;

		//I/O instructions---------------------------------------------------------------------------------------------------------------------------
		case 0x23: //Print a single unsigned int to the console
			//std::cerr << decodeParamOut(addTwoU8(arg1, arg2));
			programCounter += 3;
			break;
		case 0x24: //Print a single signed int to the console
			//std::cerr << static_cast<s16>(decodeParamOut(addTwoU8(arg1, arg2)));
			programCounter += 3;
			break;
		case 0x25: //Print a single character to the console
			//std::cerr << (char)decodeParamOut(addTwoU8(arg1, arg2));
			programCounter += 3;
			break;
		case 0x26: { //Takes a line of input and stores it from a memory address onwards
			std::string tmp = "";
			std::getline(std::cin, tmp);
			u16 memoryLocation = decodeParamOut(addTwoU8(arg1, arg2));
			for(u16 i = 0; i < tmp.length(); ++i) {memory[memoryLocation + i] = (int)tmp[i];}
			programCounter += 3;
			break; }

		//Default error------------------------------------------------------------------------------------------------------------------------------
		default:
			std::cerr << "Unknown instruction: 0x" << std::hex << (int)instruction << '\n';
			halt = true;
			break;
	}
}

//If the CPU is not halted, load the next instruction and execute it
cpuData CPU::tick() {
	memoryChange.clear();
	cpuData temp = {programCounter, memory[programCounter], halt, overflow, underflow, stack.toVector(), tempStack, registers, memoryChange};
	if(!halt) {
		OPcode = memory[programCounter];
		execute();
	}
	return temp;
}

void CPU::tick2() {
	if(!halt) {
		OPcode = memory[programCounter];
		execute();
	}
}