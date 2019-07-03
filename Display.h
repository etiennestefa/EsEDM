#ifndef DISPLAY_H
#define DISPLAY_H

#include <fstream>
#include "Location.h"
#include "Memory.h"

class Display{
private:
	string stream;
	unsigned  int len;
	Location* status;
	Location* control;
	Location* data;
	bool* emulating;
	int time;

public:
	Display(){}
	Display(Location* data,Location* status,Location* control,bool *emulating){
		this->data = data;
		this->status = status;
		this->control = control;
		this->emulating = emulating;
		stream="";
		len = 0;
		time = 0;
	}
	void reset(){
		stream="";
		len = 0;
	}
	 void refresh(){
		if(time > 0) time--;
		if(time==0&&status->getNumber()==4){
			stream = stream + ((char)data->getNumber());
			len++;
			if(len>200) stream = stream.substr(len-200,200);
			time = 3;
		}
		if(time==1)	status->setNumber(0);
	}
	void print(){
		fstream write("EsEDMDisplay.txt", fstream::out);
		if(write.is_open()){
			write<<stream;
		}
		write.close();
	}
	string getStream(){return stream;}
	void insert(string s){
		stream = stream + s;
		refresh();
	}
};

#endif
