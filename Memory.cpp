#include "Memory.h"
#include "Location.h"

using namespace std;

Location* Memory::getLocation(int position){return &v[position];}
void Memory::clear(){
	for(int i=0;i<16384;i++){
		v[i].clear();
	}
}


 