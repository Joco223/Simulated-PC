#Assembler

This is an assembler for the Simulated-PC CPU instruction set.

How to use: Run "Assembler.exe inputFile.txt outputFile"

Details on instruction arguments and what they do is in CPU Specification.txt

Comments are written by putting ";" at the start of the line, they only work at the start of the line.
Note, hex and binary numbers do not work for now, I will try and fix that.

Instruction syntax is:

instruction arg1, arg2

Arguments are seperated by a comma

List of instructions:
halt     - Instruction  0x0 - Will halt the CPU, not necesary at the end of the program
setr     - Instruction  0x1 - Set register
mread8   - Instruction  0x2 - Memory read  8 bit
mread16  - Instruction  0x3 - Memory read 16 bit
mwrite8  - Instruction  0x4 - Memory write  8 bit
mwrite16 - Instruction  0x5 - Memory write 16 bit
add      - Instruction  0x6 - Addition
sub      - Instruction  0x7 - Subtraction
mlt      - Instruction  0x8 - Multiplication
divu     - Instruction  0x9 - Unsigned division
divs     - Instruction  0xA - Signed division
mod      - Instruction  0xB - Mod
inc      - Instruction  0xC - Increment
dec      - Instruction  0xD - Decrement
clr      - Instruction  0xE - Clear register
and      - Instruction  0xF - Bitwise and
or       - Instruction 0x10 - Bitwise or
not      - Instruction 0x11 - Bitwose not
bsright  - Instruction 0x12 - Bitwise shfit right
bsleft   - Instruction 0x13 - Bitwise shift left
eql      - Instruction 0x14 - Equality
diff     - Instruction 0x15 - Different
lrgs     - Instruction 0x16 - Signed larger than             
lrgu     - Instruction 0x17 - Unsigned larger than
jmp      - Instruction 0x18 - Jump
jmrz     - Instruction 0x19 - Jump if register is 0
jmro     - Instruction 0x1A - Jump if register is 1
jmo      - Instruction 0x1B - Jump if overflow flag is on
jmu      - Instruction 0x1C - Jump if undeflow flag is on
rst      - Instruction 0x1D - Reset flags
pusht    - Instruction 0x1E - Push to temporary stack
pushs    - Instruction 0x1F - Push to stack
pops     - Instruction 0x20 - Pop top of the stack
call     - Instruction 0x21 - Read in CPU Specification.txt
ret      - Instruction 0x22 - ^
outu     - Instruction 0x23 - Output unsigned int to console
outs     - Instruction 0x24 - Output signed int to console 
outc     - Instruction 0x25 - Output chatacter to console
in       - Instruction 0x26 - Store input from console