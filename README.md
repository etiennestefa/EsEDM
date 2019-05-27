# EsEDM
Assembly Learning Platform

Etienne's Emulated Didactic Machine
is a didactic instrument to easily teach how processors do work and most of all how we can make them work better!

Read more to understand why, Come Inside EsEDM!

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
- SBT	Operative Code: 011O	
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
example: <LABEL>
  
- Branch

To make a branch programmers just need to write the jump instruction followed by '-(', label's name and then ')!'.
example: Bra-(LABEL)!

- Binary instructions such as Mov, Not, Ldr, Str, Cmp have to be written this way:

Instruction '-' Register One '-' Register Two '!'

example: Mov-R1-R2!

or

Instruction '-' Register One '-#' Immediate Value between 127 and -128 '#!'

example: Not-R1-#7#!

- Ternary Instructions such as Add, Adi, Sbt, Sbi, And have to be written this way:

Instruction '-' Register One '-' Register Two '-' Register Three '!'

example: Add-R1-R2-R3!

or

Instruction '-' Register One '-' Register Two '-#' Immediate Value between 127 and -128 '#!'

example: Sbt-R1-R1-#7#!

# EsEDMISAssembler

EsEDMISAssembler is [...] Work in progress!
