#ifndef NENGINE_INPUT
#define NENGINE_INPUT
#include <windows.h>
unsigned char *inKeys;
long inMX,inMY,inMXinc,inMYinc=0;
char inChar;
void inReset(void) {
	logPrint("[*]\tReseting variables of IO module...\n");
	for(int i=0;i<256;i++) inKeys[i] = 0;
	SetKeyboardState(inKeys);
	inMX = inMY = inMXinc = inMYinc = 0;	
}
void inInit(){
	logPrint("[*]\tInitializing IO module...\n");
	inKeys = new unsigned char [256+3];
	inReset();
}	
void inDone(){
	delete []inKeys;
}
unsigned int inKey(unsigned int scan){
	return inKeys[MapVirtualKey(scan,1)];//(inKeys[MapVirtualKey(scan,1)]&128);
}

void inGetStatus() {
//	GetKeyboardState(inKeys);
	POINT _pos;
	GetCursorPos(&_pos);		
	inMX=_pos.x;
	inMY=_pos.y;
	SetCursorPos(160,100);		
	inMXinc=(160 - _pos.x);
	inMYinc=(100 - _pos.y);
	}
#endif