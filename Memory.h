#ifndef MEMORY_H
#define MEMORY_H
#include "Location.h"
using namespace std;

class Memory{
private:
	Location v[16384];
public:
	Location& KBD_DATA = v[16383];
	Location& KBD_STATUS = v[16382];
	Location& KBD_CONTROL = v[16381];
	Location& DISP_DATA = v[16380];
	Location& DISP_STATUS = v[16379];
	Location& DISP_CONTROL = v[16378];
	void clear();
	
	
	Location* getLocation(int position);
	Location* operator []( int n){
		return this->getLocation(n);
	}
};

#endif