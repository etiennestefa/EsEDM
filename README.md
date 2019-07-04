# EsEDM Assembly Learning Platform

Etienne's Emulated Didactic Machine
is a didactic instrument to easily teach how processors do work and most of all how we can make them work better!

Read more to understand why, Come Inside EsEDM!

# MENU
 
- [EsEDM ISA](#esedm-isa)
- [EsEDM ISA Assemblative Language](#esedm-isa-assemblative-language)
- [How to write a well-made code](#how-to-write-a-well-made-code)
- [How to assemble by yourself](#how-to-assemble-by-yourself)
- [EsEDMISAssembler](#esedmisassembler)
- [Haskell's role in making EsEDMISAssembler](#haskells-role-in-making-esedmisassembler)
- [Compiling EsEDMISAssembler](#compiling-esedmisassembler)
- [Using EsEDMISAssembler](#using-esedmisassembler)
- [EsEDM](#esedm)
- [EsEDM Registers](#esedm-registers)
- [EsEDM Memory and I/O](#esedm-memory-and-io)
- [EsEDM Pipeline](#esedm-pipeline)
- [EsEDM Display](#esedm-display)
- [EsEDM Keyboard](#esedm-keyboard)
- [Compiling EsEDM](#compiling-esedm)
- [Using EsEDM](#using-esedm)
- [Enjoying EsEDM](#enjoying-esedm)


# EsEDM ISA

EsEDM ISA is the instruction set the machine is based on.

EsEDM ISA is very simple and easy-understanding. 
In facts, users are given only 16 possible commands which allow every possible computation and stimulate users brains.

The instructions are quite short, in facts they are made of 16 bits and users are shown how the assemble them so that they can have a full immersion in ISAs world.

# EsEDM ISA Assemblative Language

Programming on EsEDM you are given 16 RISC style commands which are enough for making every kind of operation

- MOV	Operative Code: 0000	
- NOT	Operative Code: 0001
- LDR	Operative Code: 0010
- STR	Operative Code: 0011
- ADD	Operative Code: 0100
- ADI	Operative Code: 0101
- SBT	Operative Code: 0110	
- SBI	Operative Code: 0111
- BRA	Operative Code: 1000 
- BEQ	Operative Code: 1001 
- BRL	Operative Code: 1010 
- BRG	Operative Code: 1011 
- BLE	Operative Code: 1100 
- BGE	Operative Code: 1101 
- AND	Operative Code: 1110 
- CMP	Operative Code: 1111 

Programmers are also allowed to use all of the 8 main 16 bits sized registers composing the machine:

- R0	Coded as: 000 
- R1	Coded as: 001 
- R2	Coded as: 010 
- R3	Coded as: 011 
- FP	Coded as: 100 
- SP	Coded as: 101 
- PC	Coded as: 110 
- IR	Coded as: 111 

# How to write a well-made code

- Labels

  Programmers can define a label putting label's name between '<' and '>'.

  example: <_LABEL_>
  
- Branch

  To make a branch programmers just need to write the jump instruction followed by '-(', label's name and then ')!'.
  
  example: Bra-(_LABEL_)!

- Binary instructions such as Mov, Not, Ldr, Str, Cmp have to be written this way:

  Instruction '-' Register One '-' Register Two '!'

  example: Mov-R1-R2!

- or

  Instruction '-' Register One '-#' Immediate Value between 127 and -128 '#!'

  example: Not-R1-#7#!

- Ternary Instructions such as Add, Adi, Sbt, Sbi, And have to be written this way:

  Instruction '-' Register One '-' Register Two '-' Register Three '!'

  example: Add-R1-R2-R3!

- or

  Instruction '-' Register One '-' Register Two '-#' Immediate Value between 15 and -16 '#!'

  example: Sbt-R1-R1-#7#!

# How to assemble by yourself

- Jump instructions:

  Operative code followed by destination's two's complement

  example: Bra-(LABEL)! turns into  1000 xxxxxxxxxxx

- Binary instruction without immediate value:

  Operative code followed by 1, Register One, Register Two, 00

  example: Mov-R1-R2! turns into 0000 1 001 010 00000

- Binary instruction with immediate value:

  Operative code followed by 0, Register One, Two's complement of the immediate value

  example: Not-R3-#1#! turns into 0001 0 011 00000001

-  Ternary instruction without immediate value:

  Operative code followed by 1, Register One, Register Two, Register Three, 00

  example: Add-R1-R2-R3! turns into 0100 1 001 010 011 00
  
 -  Ternary instruction with immediate value:

  Operative code followed by 0, Register One, Register Two, Two's complement of the immediate value

  example: Sbt-R1-R2-#-1#! turns into 0110 0 001 010 11111


# EsEDMISAssembler

EsEDMISAssembler is a C++ and Haskell based assembler.

It is able to find, identify and show to programmers errors.

Assemblative process takes two steps to prouce the output.

- Step One: Instructions, registers and immediate values are translated ad labels are inserted in a buffer.

- Step Two: Jump instructions are poperly linked with their label.

The step one process can be saved putting flag -ws that does save the EsEDMISAssemblerWorkSpace.edmws

EsEDMISAssembler is generally a file named a.edmexe where istructions are coded as '0' and '1' characters, an instruction per line. In this way new programmers can easily see and understand assemblative process.

# Haskell's role in making EsEDMISAssembler

The main goal of an assembler is to translate a list of instructions from a easily-human-understanding language to machine's language.

EsEDMISA translation can be seen as a function:

f : A -> B

where:

- A is composed by all the well written instruction in EsESEDMISA language
- B is composed by all the possible instruction in EsEDMISA

what's more, if we consider the instruction: 

i = ADD-R1-R2-R3!

f(i) can be seen as

f(i) = g(ADD) + p(R3) + h(R1) + h(R2) + h(R3)

where:

- g is the function that returns the operative code of an instruction

- p returns 1 if the argument is a register, else returns 0

- h returns the code of the register given


it is easy now to understand that function f may be easily turned into the sum of many elementary functions.

But why Haskell is useful now ?

Haskell is a functional language, so defining "mathematical" functions is easier than in other imperative languages.

In addition, there is no need to make the assembler totally in Haskell but it is easy to link Haskell's functions to other programs made in other languages using flag "-XForeignFunctionInterface" as shown in the next paragraph.


# Compiling EsEDMISAssembler

- ghc -c -XForeignFunctionInterface -O EsEDMISAFunctions.hs

- g++ -c -O EsEDMISAssembler.cpp -I "/usr/lib/ghc/include/"

- ghc -no-hs-main EsEDMISAFunctions.o EsEDMISAssembler.o -lstdc++

# Using EsEDMISAssembler

Programmers can run the assembler in three ways:

- ./EsEDMISAssembler source.edmisa

- ./EsEDMISAssembler source.edmisa output.edmexe

- ./EsEDMISAssembler -flag source.edmisa output.edmexe

# EsEDM

EsEDM is the virtual machine that is able to build programs, run them and show machine's status in real time.

It is made of:

- 8 main registers

- 5 service registers

- 16384 memory locations (I/O included)

# EsEDM Registers

EsEDM has 8 main registers:


- R0	Coded as: 000 
- R1	Coded as: 001 
- R2	Coded as: 010 
- R3	Coded as: 011 
- FP	Coded as: 100 Frame Pointer
- SP	Coded as: 101 Stack Pointer
- PC	Coded as: 110 Program Counter
- IR	Coded as: 111 Instruction Register

# EsEDM Service Registers

- RA Operand One
- RB Operand Two
- RC Memory Location's Index
- RY Operation's Result Destination
- RZ Operation's Result

# EsEDM Memory and I/O 

EsEDM includes a 16384 locations-made memory where

- From Loc 0 to Program Length the program is loaded
- Loc 16378 is reserved for Display Control (NOT EMULATED)
- Loc 16379 is reserved for Display Status
- Loc 16380 is reserved for Display Data
- Loc 16381 is reserved for Keyboard Control (NOT EMULATED)
- Loc 16382 is reserved for Keyboard Status
- Loc 16383 is reserved for Keyboard Data

Programmer is able to save program's memory at any time thanks to Print Memory button.

# EsEDM Pipeline

While running EsEDM follows this pipeline:

- Step One: Loading the instruction in the Instruction Register and setting the Program Counter
- Step Two: Decoding the instruction and setting service registers
- Step Three/Four: Aritmetic or Logic operation / Memory interface
- Step Five: Storing the result

# EsEDM Display

EsEDM includes a 200 chars display that can be used putting the chosen ASCII char into memory location 16380 a.k.a. Display Data and setting memory location Display Status as 4 (bit 2 acts as DOUT signal).

EsEDM Display also simulates a small delay in turning off DOUT signal.

Programmer is able to save program's output at any time thanks to Print Display button.

# EsEDM Keyboard

EsEDM includes also a keyboard. Programmers shall insert the character into the text box and press the Send Key button.

WARNING! Keyboard cannot be used in Run modality!

# Compiling EsEDM

EsEDM is realised on CodeBlock using wxwidgets 3.0 libraries that allow the program to be ran on all the main OSs and can easily be compiled using g++.

# Using EsEDM

EsEDM allows users to:

- Call the Assembler (assembler in the same directory and xterm needed). Key 'A',
- Load a program. Key 'L'
- Run a program (not to be used with programs that need Keyboard). Key 'R'
- Run a program one instruction per time. Key 'N'

# Enjoying EsEDM

EsEDM may be a good didactic platform thanks to its easy commands, language and ISA:

- The assemblative process can be followed or even hand-made
- It can be used as a good platform to develop embrional high level languages
- It can be used to improve low level abilities

What are you waiting for? Try Etienne's Emulated Didactic Machine!
