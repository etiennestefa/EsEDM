#include <fstream>
#include <limits.h>
#include "Location.h"
#include "Memory.h"
#include "Registers.h"
#include "EsEDM.h"


int istructionDecoder(string x){
    return 8*(x[0]-'0')+4*(x[1]-'0')+2*(x[2]-'0')+1*(x[3]-'0');
}
int registerDecoder(string x){
    return 4*(x[0]-'0')+2*(x[1]-'0')+1*(x[2]-'0');


}


int VirtualMachine::complementoADue(int len, string x){
	int result = 0;
	bool negative = false;
	if(x[0]=='1'){
		negative = true;
		for(int i = 0; i<len ; i++){
			if(x[i]=='1') x[i]='0';
			else x[i]='1';
		}
	}
	for(int i = 1; i<len ; i++){
        if(result==0) result = ((int)(x[i]-48));
		else result = result*2 + ((int)(x[i]-48));
	}
	if(negative) result = -(result+1);
	return result;
}

bool VirtualMachine::getProgramLoaded(){
	return this->programLoaded;
}
bool VirtualMachine::getProgramNotFound(){
	return this->programNotFound;
}

void VirtualMachine::load(string inputName, bool& taskend){
	fstream read(inputName, fstream::in);
	if(!read.is_open()){
		programLoaded = false;
		programNotFound = true;
		taskend= true;
		return ;
	}
	resetMemory();
	resetRegisters();
	display.reset();
	O=false;
	N=false;
	Z=false;
	int lineCounter=0;
	string instruction = "";
	while(!read.eof()){
		read>>instruction;
		memory[lineCounter]->setInstruction();
		memory[lineCounter++]->setValue(instruction);
	}
	registers.FP.setNumber(lineCounter);
	registers.SP.setNumber(lineCounter);
	programLoaded = true;
	clockCounter = 0;
	read.close();
	emulationCompleted=false;
	taskend= true;
}

void VirtualMachine::resetMemory(){
	memory.clear();
}

void VirtualMachine::resetRegisters(){
	registers.clear();
}

void VirtualMachine::run(){
	while(!emulationCompleted) clockCicle();
}

void VirtualMachine::clockCicle(){
	emulating = true;
	if(!programLoaded) {
		emulating = false;
		return;
	}
	stepOne();
	if(emulationCompleted||errorFound){
		emulating = false;
		return;
	}
	stepTwo();
	stepThreeAndFour();
	stepFive();
	emulating = false;
	clockCounter++;
	if(clockCounter==UINT_MAX){
        display.reset();
        display.insert(" SHUTTING DOWN : REACHED CLOCK LIMITS ! IF YOU RUN A I/O PROGRAM USE NEXT STEP !");
        emulationCompleted = true;
	}
}

void VirtualMachine::stepOne(){
	if(registers.FP.getNumber()<=registers.PC.getNumber()){
        emulationCompleted = true;
        return;
	}
	//PRELEVO L'ISTRUZIONE DI INDICE PC E LA POSIZIONO IN IR
	int line = registers.PC.getNumber();
	//SE IL POGRAMMA È TERMINATO ESCO
	if(!memory[line]->getIsAnInstruction()){
		emulationCompleted = true;
		return;
	}
	int instruction = memory[line]->getNumber();
	registers.IR.setNumber(instruction);
	//INCREMETO IL PC
	registers.PC.setNumber(line+1);
}

void VirtualMachine::stepTwo(){
	string instruction = "";
	{
		instruction = registers.IR.getValue();
		instructionCode = istructionDecoder(instruction.substr(0,4));
	}
	//SE OPERAZIONE BINARIA
	if((instructionCode>=0&&instructionCode<=3)||instructionCode==15){
		int r1,r2,r2Id,r1Id;
		r1Id = registerDecoder(instruction.substr(5,3));
		r1 = registers[r1Id]->getNumber();
		//SECONDO OPERANDO REGISTRO
		if(instruction[4]=='1'){
			r2Id = registerDecoder(instruction.substr(8,3));
			r2 = registers[r2Id]->getNumber();
		}
		//SECONDO OPERANDO VALORE IMMEDIATO
		else{
			r2 = complementoADue(8,instruction.substr(8,8));
		}
		//SE IL PRIMO OPERANDO È LA DESTINAZIONE
		if(instructionCode>=0&&instructionCode<=2){
			registers.RY.setNumber(r1Id);
			registers.RA.setNumber(r2);
		}
		if(instructionCode==15){
			registers.RA.setNumber(r1);
			registers.RB.setNumber(r2);
		}
		if(instructionCode==3){
			registers.RA.setNumber(r1);
			registers.RC.setNumber(r2);
		}

	}
	//SE OPERAZIONE TERNARIA
	if((instructionCode>=4&&instructionCode<=7)||instructionCode==14){
		int r1,r2,r3,rId;
		rId = registerDecoder(instruction.substr(5,3));
		r1 = rId/*registers[rId]->getNumber()*/;
		rId = registerDecoder(instruction.substr(8,3));
		r2 = registers[rId]->getNumber();
		//TERZO OPERANDO REGISTRO
		if(instruction[4]=='1'){
			rId = registerDecoder(instruction.substr(11,3));
			r3 = registers[rId]->getNumber();
		}
		//TERZO OPERANDO VALORE IMMEDIATO
		else{
			r3 = complementoADue(5,instruction.substr(11,5));
		}
		registers.RY.setNumber(r1);
		registers.RA.setNumber(r2);
		registers.RB.setNumber(r3);
	}
	//SE ISTRUZIONE DI SALTO
	if(instructionCode>=8&&instructionCode<=13){
            //display.insert(instruction);
		int destination = complementoADue(12,instruction.substr(4,12));
		//keyboard.setC(destination);
		registers.RA.setNumber(destination);
	}
}


void VirtualMachine::stepThreeAndFour(){
	switch(instructionCode){
		case 0:
			mov();
			break;
		case 1:
			nOt();
			break;
		case 2:
			ldr();
			break;
		case 3:
			str();
			break;
		case 4:
			add();
			break;
		case 5:
			adi();
			break;
		case 6:
			sbt();
			break;
		case 7:
			sbi();
			break;
		case 8:
			bra();
			break;
		case 9:
			beq();
			break;
		case 10:
			brl();
			break;
		case 11:
			brg();
			break;
		case 12:
			ble();
			break;
		case 13:
			bge();
			break;
		case 14:
			aNd();
			break;
		case 15:
			cmp();
			break;
		default:
			errorFound = true;
			break;
	}
}

void VirtualMachine :: stepFive(){
	if(instructionCode!=3&&instructionCode!=15){
		int y = registers.RY.getNumber();
		int x = registers.RZ.getNumber();
		registers[y]->setNumber(x);
	}
}


void VirtualMachine::mov(){
	int x = registers.RA.getNumber();
	registers.RZ.setNumber(x);
}

void VirtualMachine::nOt(){
	string x = registers.RA.getValue();
	for(int i = 0; i<16; i++){
		if(x[i]=='1') x[i] = '0';
		else x[i] = '1';
	}
	registers.RZ.setValue(x);
}

void VirtualMachine::ldr(){
	int x = registers.RA.getNumber();
	int w = 0;
	if(x<16384||x>=registers.FP.getNumber()){
        w = memory[x]->getNumber();
	}
	else{
        O = true;
        emulationCompleted = true;
        display.insert(" EXECUTION ARRESTED. CAUSE: STACK OVERFLOW");
	}
	registers.RZ.setNumber(w);
}

void VirtualMachine::str(){
	int x = registers.RA.getNumber();
	int y = registers.RC.getNumber();
	if(y<16384||y>=registers.FP.getNumber()){
        memory[y]->setNumber(x);
	}
	else{
        O = true;
        emulationCompleted = true;
        display.insert(" EXECUTION ARRESTED. CAUSE: STACK OVERFLOW");
	}
}


void VirtualMachine::add(){
	int x = registers.RA.getNumber();
	int y = registers.RB.getNumber();
	registers.RZ.setNumber(x+y);
}

void VirtualMachine::adi(){
	int x = registers.RA.getNumber();
	int y = registers.RB.getNumber();
	int z = ((int)x) +((int)y);
	if(z==0) Z = 1;
	if(z<0) N = 1;
	if(z>SHRT_MAX||z<SHRT_MIN) O = 1;
	else registers.RZ.setNumber(x+y);
}

void VirtualMachine::sbt(){
	int x = registers.RA.getNumber();
	int y = registers.RB.getNumber();
	registers.RZ.setNumber(x-y);
}

void VirtualMachine::sbi(){
	int x = registers.RA.getNumber();
	int y = registers.RB.getNumber();
	int z = ((int)x) -((int)y);
	if(z==0) Z = 1;
	if(z<0) N = 1;
	if(z>SHRT_MAX||z<SHRT_MIN) O = 1;
	else registers.RZ.setNumber(x-y);
}

void VirtualMachine::bra(){
	int x = registers.RA.getNumber();
	registers.RY.setNumber(6);
	registers.RZ.setNumber(x);
}

void VirtualMachine::beq(){
	if(Z) bra();
	else{
		int x = registers.PC.getNumber();
		registers.RY.setNumber(6);
		registers.RZ.setNumber(x);
	}
}

void VirtualMachine::brl(){
	if(!Z&&N) bra();
	else{
		int x = registers.PC.getNumber();
		registers.RY.setNumber(6);
		registers.RZ.setNumber(x);
	}
}

void VirtualMachine::brg(){
	if(!Z&&!N) bra();
	else{
		int x = registers.PC.getNumber();
		registers.RY.setNumber(6);
		registers.RZ.setNumber(x);
	}
}

void VirtualMachine::ble(){
	if(Z||N) bra();
	else{
		int x = registers.PC.getNumber();
		registers.RY.setNumber(6);
		registers.RZ.setNumber(x);
	}
}

void VirtualMachine::bge(){
	if(Z||!N) bra();
	else{
		int x = registers.PC.getNumber();
		registers.RY.setNumber(6);
		registers.RZ.setNumber(x);
	}
}

void VirtualMachine::aNd(){
	string x = registers.RA.getValue();
	string y = registers.RB.getValue();
	for(int i = 0; i<16; i++){
		if(x[i]=='1'&&y[i]=='1') x[i] = '1';
		else x[i] = '0';
	}
	registers.RZ.setValue(x);
}

void VirtualMachine::cmp(){
	int x = registers.RA.getNumber();
	int y = registers.RB.getNumber();
	long int z = ((long int)x) -((long int)y);
	if(z==0) Z = 1;
	else Z = 0;
	if(z<0) N = 1;
	else N = 0;
	if(z>INT_MAX||z<INT_MIN) O = 1;
	else O = 0;
}


void VirtualMachine::printMemory(){
	fstream write("EsEDMemory.txt", fstream::out);
	if(write.is_open()){
		for(int i = 0; i< 16384; i++){
			write<<"Loc ["<<i<<"]"<<'\t'<<'\t'<<memory[i]->getValue()<<'\t'<<'\t'<<memory[i]->getNumber()<<endl;
		}
	}
	write.close();
}

bool VirtualMachine::getZ(){return Z;}
bool VirtualMachine::getO(){return O;}
bool VirtualMachine::getN(){return N;}

string VirtualMachine::getMemoryValue(int pos){ return memory[pos]->getValue();}
int VirtualMachine::getMemoryNumber(int pos){return memory[pos]->getNumber();}
string VirtualMachine::getRegisterValue(int pos){return registers[pos]->getValue();}
int VirtualMachine::getRegisterNumber(int pos){return registers[pos]->getNumber();}
unsigned int VirtualMachine::getClockCounter (){return clockCounter;}









