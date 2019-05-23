--ETIENNE's EMULATED DIDACTIC MACHINE INSTRUCTION SET ARCHITECTURE
--2019 STEFANO SAVINO


module Foo where

foreign export ccall toMaj :: Char -> Char
foreign export ccall codeTranslator :: Char -> Char -> Char -> Int
foreign export ccall registerTranslator :: Char -> Char -> Int
foreign export ccall isABranchInstruction :: Char -> Char -> Char -> Char -> Int

serviceCodeTranslator :: String -> Int
serviceCodeTranslator "MOV" = 0
serviceCodeTranslator "NOT" = 1
serviceCodeTranslator "LDR" = 2
serviceCodeTranslator "STR" = 3
serviceCodeTranslator "ADD" = 4
serviceCodeTranslator "ADI" = 5
serviceCodeTranslator "SBT" = 6
serviceCodeTranslator "SBI" = 7
serviceCodeTranslator "BRA" = 8
serviceCodeTranslator "BEQ" = 9
serviceCodeTranslator "BRL" = 10
serviceCodeTranslator "BRG" = 11
serviceCodeTranslator "BGE" = 12
serviceCodeTranslator "BLE" = 13
serviceCodeTranslator "AND" = 14
serviceCodeTranslator "CMP" = 15
serviceCodeTranslator x = -1

seviceCodeStringMaker :: Char -> Char -> Char -> String
seviceCodeStringMaker x y z = [x,y,z]

codeTranslator :: Char -> Char -> Char -> Int
codeTranslator x y z = serviceCodeTranslator (seviceCodeStringMaker (toMaj (x)) (toMaj (y)) (toMaj (z)) )


--Registers
serviceRegisterTranslator :: String -> Int
serviceRegisterTranslator "R0" = 0
serviceRegisterTranslator "R1" = 1
serviceRegisterTranslator "R2" = 2
serviceRegisterTranslator "R3" = 3
serviceRegisterTranslator "FP" = 4
serviceRegisterTranslator "SP" = 5
serviceRegisterTranslator "PC" = 6
serviceRegisterTranslator "IR" = 7
serviceRegisterTranslator x = -1

seviceRegisterStringMaker :: Char -> Char -> String
seviceRegisterStringMaker x y = [x,y]

registerTranslator :: Char -> Char -> Int
registerTranslator x y = serviceRegisterTranslator (seviceRegisterStringMaker (toMaj x) (toMaj y))

toMaj :: Char -> Char
toMaj 'a' = 'A'
toMaj 'b' = 'B'
toMaj 'c' = 'C'
toMaj 'd' = 'D'
toMaj 'e' = 'E'
toMaj 'f' = 'F'
toMaj 'g' = 'G'
toMaj 'h' = 'H'
toMaj 'i' = 'I'
toMaj 'j' = 'J'
toMaj 'k' = 'K'
toMaj 'l' = 'L'
toMaj 'm' = 'M'
toMaj 'n' = 'N'
toMaj 'o' = 'O'
toMaj 'p' = 'P'
toMaj 'q' = 'Q'
toMaj 'r' = 'R'
toMaj 's' = 'S'
toMaj 't' = 'T'
toMaj 'u' = 'U'
toMaj 'v' = 'V'
toMaj 'w' = 'W'
toMaj 'x' = 'X'
toMaj 'y' = 'Y'
toMaj 'z' = 'Z'
toMaj x = x

serviceIsABranchInstruction :: String -> Int
serviceIsABranchInstruction "1000" = 1
serviceIsABranchInstruction "1001" = 1
serviceIsABranchInstruction "1010" = 1
serviceIsABranchInstruction "1011" = 1
serviceIsABranchInstruction "1100" = 1
serviceIsABranchInstruction "1101" = 1
serviceIsABranchInstruction x = 0

serviceIsABranchInstructionStringMaker :: Char -> Char -> Char -> Char -> String
serviceIsABranchInstructionStringMaker x y z w = [x,y,z,w] 

isABranchInstruction :: Char -> Char -> Char -> Char -> Int
isABranchInstruction x y z w = serviceIsABranchInstruction (serviceIsABranchInstructionStringMaker x y z w)
