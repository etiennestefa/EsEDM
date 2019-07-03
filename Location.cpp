#include <string>
#include "Location.h"

using namespace std;


void Location::clear(){
	number = 0;
	value = "0000000000000000";
	isAnInstruction = false;
}
void Location::setNumber(int number){
	this->number = number;
	refreshValue();
}
void Location::setValue(string value){
	this->value = value;
	refreshNumber();
}
void Location::setInstruction(){isAnInstruction = true;}
void Location::setNotInstruction(){isAnInstruction = false;}
int Location::getNumber(){return number;}
string Location::getValue(){return value;}
bool Location::getIsAnInstruction(){return isAnInstruction;}
void Location::refreshNumber(){
	/*number = 0;
	if(value[0]=='0'){
		for(int i=1; i<16; i++){
			number = number*2 + ((int) (value[i]-'0'));
		}
	}
	else{
		string s = value;
		for(int i=15; i>=0; i--) {
			if(s[i]=='0') s[i] = '1';
			else s[i] = 0;
		}
		for(int i=1; i<16; i++){
			number = number*2 + ((int) (s[i]-'0'));
		}
		number = 0-number-1 ;
	}*/
	number=complementoADue(16, value);
}
void Location::refreshValue(){
	int tmp = number;
	if (number<0) tmp = -tmp -1;
	for(int i = 15; i>=0; i--){
		value[i] = (char)(tmp%2 + '0');
		tmp = tmp/2;
	}
	if (number<0){
		for(int i = 15; i>=0; i--){
			if(value[i]=='0') value[i] = '1';
			else value[i] = '0';
		}
	}

}

int Location::complementoADue(int len, string x){
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
