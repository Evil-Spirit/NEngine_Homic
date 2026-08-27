#ifndef NENGINE_MISCELLANEOUS
#define NENGINE_MISCELLANEOUS
#include <windows.h>
#include <gl\\gl.h>
#include <gl\\glu.h>
#include "engVector.h"
#include <stdio.h>
void glSetViewMatrix(tVector ang,tVector pos) {
	glLoadIdentity();
	glRotated(ang.z,0,0,1);
	glRotated(ang.x,1,0,0);
	glRotated(ang.y,0,1,0);
	glTranslated(pos.x,pos.y,pos.z);
}
void glSwapState(GLenum s) {
	if (glIsEnabled(s)) glDisable(s); else glEnable(s);
}

bool glIsExtensionSupported(char *s) {
	if (strstr((const char*)glGetString(GL_EXTENSIONS),s)) return true; else return false;
}

void Swap(unsigned char *a, unsigned char *b) {*a^=*b;*b^=*a;*a^=*b;}
void Swap(char *a, char *b) {*a^=*b;*b^=*a;*a^=*b;}
void Swap(int *a, int *b) {*a^=*b;*b^=*a;*a^=*b;}

#endif 