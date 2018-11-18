#Assembler

This is an assembler for the Simulated-PC CPU instruction set.

How to use: Run "Assembler.exe inputFile.txt outputFile".

Details on instruction arguments and what they do is in CPU Specification.txt.

Comments are written by putting ";" at the start of the line, they only work at the start of the line.
Note, hex and binary numbers do not work for now, I will try and fix that.

To load a program into Simulated-PC, put the assembled file name as the second executable argument.

Instruction syntax is:

instruction arg1, arg2

Instructions are in Assembler Instructions.txt