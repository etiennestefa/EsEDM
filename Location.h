#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <math.h>

using namespace std;

class Location{
private:
	int number;
	string value;
	bool isAnInstruction;
	void refreshNumber();
	void refreshValue();
public:
	Location(){
		number = 0;
		value = "0000000000000000";
		isAnInstruction = false;
	}
	void clear();
	void setNumber(int number);
	void setValue(string value);
	void setInstruction();
	void setNotInstruction();
	int getNumber();
	string getValue();
	bool getIsAnInstruction();
	int complementoADue(int len, string x);

};

#endif
