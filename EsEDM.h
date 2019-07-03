#ifndef ESEDM_H
#define ESEDM_H

#include "Location.h"
#include "Memory.h"
#include "Registers.h"
#include "Display.h"
#include "Keyboard.h"

class VirtualMachine {
private:
	//COMPONENTI
	Memory memory;
	Registers registers;

	bool Z;
	bool N;
	bool O;
	//ALU
	void mov();
	void nOt();
	void add();
	void adi();
	void sbt();
	void sbi();
	void cmp();
	void aNd();
	void bra();
	void brl();
	void brg();
	void ble();
	void bge();
	void blg();
	void beq();
	void ldr();
	void str();
	//UTILITÀ
	int instructionCode;
	bool programLoaded;
	bool programNotFound;
	bool emulationCompleted;
	bool errorFound;
	bool emulating;
	//DA SISTEMARE
	int complementoADue(int len, string x);
	//PIPELINE
	void stepOne();
	void stepTwo();
	void stepThreeAndFour();
	void stepFive();
    unsigned int clockCounter;
public:
	Keyboard keyboard;
	Display display;
	VirtualMachine(){
		keyboard = Keyboard(&memory.KBD_DATA,&memory.KBD_STATUS,&memory.KBD_CONTROL);
		display = Display(&memory.DISP_DATA,&memory.DISP_STATUS,&memory.DISP_CONTROL,&emulating);
		Z = false;
		N = false;
		O = false;
		instructionCode = 0;
		programLoaded = false;
		programNotFound = false;
		emulationCompleted = false;
		errorFound = false;
	}
	bool getEmulationCompleted(){return emulationCompleted;}
	bool getProgramLoaded();
	bool getProgramNotFound();
	bool getZ();
	bool getO();
	bool getN();
	void load(string inputName,bool& taskend);
	void resetMemory();
	void resetRegisters();
	void run();
	void clockCicle();
	void printMemory();
	string getMemoryValue(int pos);
	string getRegisterValue(int pos);
	int getMemoryNumber(int pos);
	int getRegisterNumber(int pos);
    unsigned int getClockCounter();
};


#endif
