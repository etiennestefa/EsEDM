/***************************************//*

EsEDMISAASSEMBLER
CODED BY: STEFANO SAVINO
16/05/2019

*//***************************************/

//HEADERS

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include "EsEDMISAFunctions_stub.h"



using namespace std;

/*****************************************/

string stringToMaj(string x);


class Label{
private:
	string name;
	unsigned int link;
	Label* next;
public:
	Label(string name, unsigned int link, Label* next = nullptr){
		unsigned int l = name.length();		
		this->name = stringToMaj(name);
		this->link = link;
		this->next = nullptr;
	}
	void setNext(Label* next){
		this->next = next;
	}
	string getName(){return name;}
	unsigned int getLink(){return link;}
	Label* getNext(){return next;}
};


class LabelBuffer{
private:
	unsigned int n;
	Label* head;
	Label* current;
public:
	LabelBuffer(){
		n = 0;
		head = nullptr;
		current = nullptr;
	}
	void insert(string name, unsigned int link){
		n++;
		if(!head){
			head = new Label(name,link);
			current = head;
		}
		else{
			current->setNext(new Label(name,link));
			current = current->getNext();
		}
	}
	unsigned int getLink(string name, bool& found ){
		if(n!=0) {
			current = head;
			while(current!=nullptr){
				if(current->getName()==name){
					found = true;
					return current->getLink();
				}
				current = current->getNext();
			}
		}
		found = false;
		return 0;
	}
	
};

//CLASSES

static LabelBuffer buffer = LabelBuffer();
bool assembled;
bool wsmantain;

/*****************************************/

//PROTOTYPES


void assembler(string inputName,string outputName);
string translator(string instruction, bool& traductionSucces, unsigned int& errorPosition, unsigned int& errorType, bool& labelFound, unsigned int& effectiveLine);
string warningText(unsigned int errorType);
string traductionStringMaker (int len, int argument);
string stringToMaj(string x);
string complInt(int len, long int x, bool& transformable);
bool isAValue(string x);
int getValue(string x);

/*****************************************/

//MAIN

int main (int argc, char** argv){
	hs_init(&argc, &argv);
	assembled = false;
	wsmantain = false;
	if(argc <= 1) cout<<"MISSING ARGUMENT!"<<endl;
	if(argc == 2) assembler(argv[1],"a.edmexe");
	if(argc == 3) assembler(argv[1],argv[2]);
	if(argc == 4){
		if(string(argv[1])=="-ws"){
			wsmantain = true;
			assembler(argv[2],argv[3]);
		}
		else cout<<"EXPECTED A FLAG FOLLOWED BY INPUT AND OUTPUT PATH"<<endl;
	}
	if(argc > 4)  cout<<"TOO MANY ARGUMENTS GIVEN!"<<endl;
	if(assembled) cout<<"SOURCE ASSEMBLED"<<endl;
	else cout<<"ASSEMBLER FAILED"<<endl;
	//cout<<argc<<" "<<argv[0]<<" "<<argv[1]<<endl;
	hs_exit();
	return 0;
}

/*****************************************/

//FUNCTIONS & METHODS

void assembler(string inputName,string outputName){
	fstream read(inputName, fstream::in);
	fstream write("EsEDMISAssemblerWorkSpace.eaws", fstream::out);
	if(!read.is_open()){
		cout<<"ERROR: "<<inputName<< " FILE NOT FOUND!"<<endl;
		//cout<<"NOW CLOSING THE ASSEMBLER"<<endl;
		return ;
	}
	if(!write.is_open()){
		cout<<"ERROR: FILE "<<"EsEDMISAssemblerWorkSpace.eaws"<<" NOT OPENED!"<<endl;
		//cout<<"NOW CLOSING THE PROGRAMME"<<endl;
		return ;
	}
	unsigned int line = 1;
	unsigned int effectiveLine = 0;
	string instruction = "";
	bool errorAbsence = true;
	bool correctInput = true;
	unsigned int errorPosition=0;
	unsigned int errorType;
	unsigned int errorCounter = 0;
	bool labelFound = false;
	string translation = "";
	//FIRST PASS
	while(!read.eof()){
		errorAbsence = true;
		labelFound = false;
		errorType = 0;
		instruction = "";
		read>>instruction;
		translation =translator(instruction,errorAbsence,errorPosition,errorType,labelFound,effectiveLine); 
		if(errorAbsence){
			if(!labelFound) write<<translation<<endl;
		} 
		else{
			correctInput = false;
			errorCounter++;
			unsigned int len = instruction.length();
			cout<<"ERROR n. "<<errorCounter<<") FOUND IN LINE ("<<line<<":"<<errorPosition<<")"<<endl;
			cout<<instruction.substr(0,20);
			if(len>20) cout<<"...";
			cout<<endl;
			if(errorPosition<20){
				for(int i=0;i<errorPosition;i++) cout<<" ";
				cout<<"A"<<endl;
				for(int i=0;i<errorPosition;i++) cout<<" ";
				cout<<"|"<<endl;
			}
			else cout<<endl;	
			cout<<"Error description : "<<warningText(errorType)<<endl<<endl;
			write<<"ERROR n. "<<errorCounter<<endl;
		}
		line++;
		if(!labelFound) effectiveLine++;
	}
	read.close();
	write.close();
	//SECOND PASS
	if(correctInput){
		fstream read(inputName, fstream::in);
		fstream read2("EsEDMISAssemblerWorkSpace.eaws", fstream::in);
		fstream write(outputName , fstream::out);
		if(!read.is_open()){
			cout<<"ERROR: "<<inputName<< "FILE NOT FOUND!"<<endl;
			cout<<"NOW CLOSING THE ASSEMBLER"<<endl;
			return ;
		}
		if(!read2.is_open()){
			cout<<"ERROR: "<<"EsEDMISAssemblerWorkSpace.eaws"<< "FILE NOT FOUND!"<<endl;
			cout<<"NOW CLOSING THE ASSEMBLER"<<endl;
			return ;
		}
		if(!write.is_open()){
			cout<<"ERROR: FILE"<<outputName<<" NOT OPENED!"<<endl;
			cout<<"NOW CLOSING THE PROGRAMME"<<endl;
			return ;
		}
		line = 1;
		errorPosition = 5;
		string binaryInstruction;
		while(!read2.eof()){
			labelFound = true;
			instruction = "";
			binaryInstruction = "";
			read>>instruction;
			while(instruction[0]=='<') read>>instruction;
			read2>>binaryInstruction;
			unsigned int len = binaryInstruction.length();
			if(isABranchInstruction(binaryInstruction[0],binaryInstruction[1],binaryInstruction[2],binaryInstruction[3])){
				string label = stringToMaj(binaryInstruction.substr(4,len-4));
				int link = buffer.getLink(label ,labelFound);
				if(!labelFound){
					len = instruction.length();
					errorCounter++;
					cout<<"ERROR n. "<<errorCounter<<") FOUND IN LINE ("<<line<<":"<<errorPosition<<")"<<endl;
					cout<<instruction.substr(0,20);
					if(len>20) cout<<"...";
					cout<<endl;
					if(errorPosition<20){
						for(int i=0;i<errorPosition;i++) cout<<" ";
						cout<<"A"<<endl;
						for(int i=0;i<errorPosition;i++) cout<<" ";
						cout<<"|"<<endl;
					}
					else cout<<endl;	
					cout<<"Error description : "<<warningText(14)<<endl<<endl;
				}
				else {
					bool tmp=true;
					string num = complInt (12,link,tmp);
					write<<binaryInstruction.substr(0,4)+num<<endl;
				}
			}
			else write<<binaryInstruction<<endl;
			line++;
		}
		read.close();
		read2.close();
		write.close();
	}
	if(!wsmantain){int done = system("rm -r EsEDMISAssemblerWorkSpace.eaws");}
	if(errorAbsence) assembled = true;
}

/////////////////////////////////////////////WARNINGS



string warningText(unsigned int errorType){
	switch(errorType){
		case 1:
			return "expected '!' at the end of the instruction"; 
			break;
		case 2:
			return "found unknown instruction here"; 
			break;
		case 3:
			return "expected '-' here"; 
			break;
		case 4:
			return "should be found a register here"; 
			break;
		case 5:
			return "should be found a register or a value here"; 
			break;
		case 6:
			return "found a value too big for this instruction"; 
			break;
		case 7:
			return "expected '>' at the end of the label declaration"; 
			break;
		case 8:
			return "missing label's name"; 
			break;
		case 9:
			return "expected '(' followed by a label here"; 
			break;
		case 10:
			return "expected a label followed by ')' here"; 
			break;
		case 11:
			return "expected a label's name here"; 
			break;
		case 12:
			return "expected '<' here"; 
			break;
		case 13:
			return "label already defined"; 
			break;
		case 14:
			return "found jump to a label not defined befoe"; 
			break;
		default:
			return "not able to recognize this mistake";
			break;
	}
}





/////////////////////////////////////////////TRANSLATOR





















string translator(string instruction, bool& traductionSucces, unsigned int& errorPosition, unsigned int& errorType, bool& labelFound, unsigned int& effectiveLine){
	int len = instruction.length();
	string traduction = "";
	if(instruction[len-1]!='!'){
		if(instruction[0]=='<'){
			if(instruction[len-1]!='>'){
				errorPosition = len-1;
				errorType = 7;
				traductionSucces = false;
				return "";
			}
			if(len!=2){
				/*bool labelAlredyDefined = false;
				int c = buffer.getLink(stringToMaj(instruction.substr(1,len-2)),labelAlredyDefined);
				if(labelAlredyDefined){
					errorPosition = 2;
					errorType = 13;
					traductionSucces = false;
					return "";
				}
				else{*/
					buffer.insert(instruction.substr(1,len-2),effectiveLine);
					labelFound = true;
					return "";
				/*}*/
					
			} 
			else{
				errorPosition = 1;
				errorType = 8;
				traductionSucces = false;
				return "";
			}
		}
		else{
			if(instruction[len-1]=='>'){
				errorPosition = 0;
				errorType = 12;
				traductionSucces = false;
				return "";
			}
			else{
				errorPosition = len-1;
				errorType = 1;
				traductionSucces = false;
				return "";
			}	
		}	
	}
	//DECODIFICA IL CODICE OPERATIVO
	int code = codeTranslator(instruction[0],instruction[1],instruction[2]);
	if(code==-1) {
		errorPosition = 0;
		errorType = 2;
		traductionSucces = false;
		return "";
	}
	traduction = traduction + traductionStringMaker(4,code);
	//PROSEGUE LUNGO IL COMANDO
	if(instruction[3]!='-'){
		errorPosition = 3;
		errorType = 3;
		traductionSucces = false;
		return "";
	}
	//IN CASO DI ISTRUZIONE DI SALTO
	if(code>=8&&code<=13){
		//CONTROLLA EVENTUALI ERRORI
		if(instruction[4]!='('){
			errorPosition = 5;
			errorType = 9;
			traductionSucces = false;
			return "";
		}
		if(instruction[len-2]!=')'){
			errorPosition = len-2;
			errorType = 10;
			traductionSucces = false;
			return "";
		}
		if(len==7){
			errorPosition = 5;
			errorType = 11;
			traductionSucces = false;
			return "";
		}
		//SE IL SALTO È CORRETTO
		traduction = traduction + instruction.substr(5,len-7);
	}
	else{
		//PREPARA IL BIT DI SCELTA ULTIMO OPERANDO
		traduction = traduction + 'X';
		//DECODIFICA IL PRIMO REGISTRO
		int r1 = registerTranslator(instruction[4], instruction[5]);
		if(r1==-1) {
			errorPosition = 4;
			errorType = 3;
			traductionSucces = false;
			return "";
		}
		//SE CORRETTO PROSEGUE CON IL COMANDO
		traduction = traduction + traductionStringMaker(3,r1);
		if(instruction[6]!='-'){
			errorPosition = 6;
			errorType = 3;
			traductionSucces = false;
			return "";
		}
		//SE OPERAZIONE TERNARIA
		if((code>=4&&code<=7)||code==14){
			//DECODIFICA IL SECONDO REGISTRO
			int r2 = registerTranslator(instruction[7], instruction[8]);
			if(r2==-1) {
				errorPosition = 7;
				errorType = 4;
				traductionSucces = false;
				return "";
			}
			traduction = traduction + traductionStringMaker(3,r2);
			//SE CORRETTO PROSEGUE CON IL COMANDO
			if(instruction[9]!='-'){
				errorPosition = 9;
				errorType = 3;
				traductionSucces = false;
				return "";
			}
			//OTTIENE IL TERZO OPERANDO
			int r3 = registerTranslator(instruction[10], instruction[11]);
			//SE NON È UN REGISTRO
			if(r3==-1){
				//VERIFICO SE È UN VALORE IMMEDIATO
				if(isAValue(instruction.substr(10,len-11))){
					traduction[4] = '0';
					//LO ACQUISISCO
					int val = getValue(instruction.substr(11,len-13));
					bool possibleValue = true;
					string value = complInt(5,val, possibleValue);
					if(!possibleValue){
						errorPosition = 11;
						errorType = 6;
						traductionSucces = false;
						return "";
					}
					traduction = traduction + value;
				}
				else{
					errorPosition = 10;
					errorType = 4;
					traductionSucces = false;
					return "";
				}
			}
			//SE È UN REGISTRO
			else{
				traduction[4] = '1';
				traduction = traduction + traductionStringMaker(3,r3)+"00";
			}
		}
		//OPERAZIONI BINARIE
		if((code>=0&&code<=3)||code==15){
			int r2 = registerTranslator(instruction[7], instruction[8]);
			if(r2==-1){
				//VERIFICO SE È UN VALORE IMMEDIATO
				if(isAValue(instruction.substr(7,len-8))){
					traduction[4] = '0';
					//LO ACQUISISCO
					int val = getValue(instruction.substr(8,len-10));
					bool possible = true;
					string value = complInt(8,val, possible);
					if(!possible){
						errorPosition = 8;
						errorType = 6;
						traductionSucces = false;
						return "";
					}
					traduction = traduction + value;
				}
				else{
					//cout<<"WWWWWWW";
					errorPosition = 8;
					errorType = 5;
					traductionSucces = false;
					return "";
				}
			}
			else{
				traduction[4]='1';
				traduction = traduction + traductionStringMaker(3,r2)+"00000";
			}
		}
	}
	return traduction;	
}


/////////////////////////////////////////////////////FUNCTIONS









string traductionStringMaker (int len, int argument){
	string result = "";
	int counter = 0;
	if(argument<0){
		argument = pow(2,len)+argument;
	}
	
	while(argument!=0){
		//if(counter==len)cout<<"found logic error!";
		result =  ((char)('0'+(argument%2))) + result;
		argument = argument/2;
		counter++;
	}
	while (counter<len){
		result = '0' + result;
		counter++;
	}
	
		
	return result;
}

string stringToMaj(string x){
	unsigned int len = x.length();
	for(unsigned int i = 0; i<len; i++){
		char y =  toMaj(x[i]);
		x[i] = y;
	}
	return x;
}

bool isAValue(string x){
	int len = x.length(); 
	if(len<3) return false;
	if(x[0]!='#'&&x[len-1]!='#'){
		for(int i=0; i<len-2; i++){
			if(i==1 && x[i]!='-' &&(x[i]<'0' && x[i]>'9'));
			if(i!=0 && x[i]<'0' && x[i]>'9') return false;
		}
	}
	return true;
}

int getValue(string x){
	int len = x.length();
	int result = 0;
	bool isNegative = false;
	if(x[0]=='-'){
		isNegative = true;
	}
	if(!isNegative) result = result*10 + (int)(x[0]-'0');
	for(int i=1; i<len; i++){
		result = result*10 + (int)(x[i]-'0');
	}
	if(isNegative){
		result *= -1;
	}
	return result;
}

string complInt(int len, long int x, bool& transformable){
	if(x>=pow(2,len-1)){
		transformable = false;
		return "";
	}
	if(x<-pow(2,len-1)-1){
		transformable = false;
		return "";
	}
	string result = "";
	/*
	if(x>=0){
		result = result + '0';
	} */
	/*if(x<0){
		//result = result + '1';
		x=pow(2,len-1)+x;
		//result = result + traductionStringMaker(len-1,x);
	}*/
	result = result + traductionStringMaker(len,x);
	return result;
}



















/*****************************************/

//END OF THE PROGRAMME

/*****************************************/
