#ifndef NENGINE_VARIABLES 
#define NENGINE_VARIABLES 
#include <windows.h>

double	engFov			= 90;
double	engMinZ			= 1.0f;
double	engMaxZ			= 4096.0f;
int		engResX			= 320;
int		engResY			= 200;
int		engBpp			= 16;
bool	engBil			= true; 
bool	engTri			= true; 
bool	engMip			= true; 
bool	engFScr			= true;
bool	engActive		= true;

double	FPSCount		= 0;
double	FPSLast			= 0;
double	movStep			= 5;
double	rotStep			= 2;
char	FPSString[80]	="\0"; 

HDC			engDC=NULL;								
HGLRC		engGLRC=NULL;									
HWND		engWnd=NULL;									
HINSTANCE	engInstance;									

#include "engImage.h"

textureList	engTexturesList;

#endif	