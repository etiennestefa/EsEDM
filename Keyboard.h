#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Location.h"
#include "Memory.h"

class Keyboard{
private:
	char c;
	Location* status;
	Location* control;
	Location* data;

public:
	Keyboard(Location* data,Location* status,Location* control){
	    this->data=data;
	    this->control=control;
	    this->status=status;
	    c = 'A';
	}
	Keyboard(){};
	void setC(char c){
		this->c=c;
		data->setNumber((int)c);
		status->setNumber(2);
	}
};
 #endif
