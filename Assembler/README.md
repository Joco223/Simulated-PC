
# Simulated PC - Assembler

This is an assembler for the S-PC CPU instruction set.

- [How to Use](#how-to-use)
- [The Instruction Set](#the-instruction-set)
    - [Comments](#comments)
    - [Getting Detailed](#getting-detailed)
        - [Information](#information)
        - [Labels](#labels)
        - [Loading Programs](#loading-programs)
        - [Instruction Syntax](#instruction-syntax)
    - [**The Instruction List**](#the-instruction-list)

# How to use
Using the SPC Assembler is done on the command line.

`C:>assembler inputFile.txt outputFile`

`$ assembler inputFile.txt outputFile`

# The Instruction Set
The S-PC CPU Instruction Set, also referred to as the SPC InSet, or InSet, is the list of all instructions, what they do, and how they are used.

## Comments
Comments are written by putting `;` at the start of the comment:
```
; This is a comment! The assembler ignore this segment of code and keeps going.
; Note that after a semicolon is detected on a line, no other code is compiled
;   until the start of the next line.
```

Multi-line comments will be added in the near future.

## Getting Detailed
### Information

To use Hexadecimal and Binary numbers, prefix them with `0x` or `0b`, respectively.

```
; Hex
pusht 0xa8c425f
pusht 0xA8C425F ; Uppercase also works!

; Binary
pusht 0b0110110
```

### Labels
Labels are defined as such: `label <name>`

They point to the instruction that immediately follows.

You can use labels as instruction arguments, but it is only for 16-bit arguments (because they specify a memory address).

```
label myLabel   ; Define a label named "myLabel"
pusht 10        ; Push <10> onto a temporary stack.
jmp myLabel     ; Jumps to <myLabel>, and immediately executes `pusht 10`.
```

### Loading Programs
To load a program into SPC, put the *assembled* filename as the second command-line argument to the program.

`C:>spc outputFile`

`$ spc outputFile`

### Instruction Syntax
The syntax for Instructions in SPC Assembly is shown below:

`instruction arg1, arg2, arg3, ...`

Arguments are separated by a comma, and the white space after it is not required.

## The Instruction List
And voila. The masterful list of every SPC Assembly InSet Instruction!

```
 0x0 halt      Will halt the CPU, not necesary at the end of the program
 0x1 setr      Set register
 0x2 mread8    Memory read  8 bit
 0x3 mread16   Memory read 16 bit
 0x4 mwrite8   Memory write  8 bit
 0x5 mwrite16  Memory write 16 bit
 0x6 add       Addition
 0x7 sub       Subtraction
 0x8 mlt       Multiplication
 0x9 divu      Unsigned division
 0xA divs      Signed division
 0xB mod       Mod
 0xC inc       Increment
 0xD dec       Decrement
 0xE clr       Clear register
 0xF and       Bitwise and
0x10 or        Bitwise or
0x11 not       Bitwose not
0x12 bsright   Bitwise shfit right
0x13 bsleft    Bitwise shift left
0x14 eql       Equality
0x15 diff      Different
0x16 lrgs      Signed larger than             
0x17 lrgu      Unsigned larger than
0x18 jmp       Jump
0x19 jmrz      Jump if register is 0
0x1A jmro      Jump if register is 1
0x1B jmo       Jump if overflow flag is on
0x1C jmu       Jump if undeflow flag is on
0x1D rst       Reset flags
0x1E pusht     Push to temporary stack
0x1F pushs     Push to stack
0x20 pops      Pop top of the stack
0x21 call      Read in CPU Specification.txt
0x22 ret       ^
0x23 outu      Output unsigned int to console
0x24 outs      Output signed int to console
0x25 outc      Output chatacter to console
0x26 in        Store input from console
```
