#ifndef REGISTERS_H
#define REGISTERS_H

#include "Location.h" 

class Registers{
private:
	Location r[13];
public:
	Location& R0 = r[0];
	Location& R1 = r[1];
	Location& R2 = r[2];
	Location& R3 = r[3];
	Location& FP = r[4];
	Location& SP = r[5];
	Location& PC = r[6];
	Location& IR = r[7];
	Location& RA = r[8];//OPERANDO AL 1
	Location& RB = r[9];//OPERANDO AL 2
	Location& RC = r[10];//DESTINAZIONE IN MEMORIA
	Location& RY = r[11];//DESTINAZIONE
	Location& RZ = r[12];//RISULTATO
	void clear(){
		for(int i = 0; i<13; i++){
			r[i].clear();
		}
	}
	Location* getRegister(int position){
		return &(this->r[position]);
	}
	Location* operator []( int n){
		return this->getRegister(n);
	}
};

#endif