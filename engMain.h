#ifndef NENGINE_MAIN_FRAMEWORK
#define NENGINE_MAIN_FRAMEWORK
#include "engTypes.h"
#include "engVars.h"
#include "engText.h"
#include "engInput.h"
#include "engResource.h"

void mainEngineExit(void);

#include "engConsole.h"
bool usrSceneInit(void);
bool usrSceneDone(void);
void usrSceneReSize(int width, int height);
void usrSceneDraw(void);
void usrSceneChange(unsigned int time);
void usrMouseDown(unsigned int but);
void usrMouseUp(unsigned int but);
UINT mainTimerID;
LRESULT CALLBACK mainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void mainEngineShutdown(void) {
	logPrint("\t***Main Shutdowning***\n");
	timeKillEvent(mainTimerID);
	if (engFScr) {
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
	if (engGLRC) {
		if (!wglMakeCurrent(NULL,NULL))
			logPrint("[!]\tReleasing Of DC and RC failed.\n");

		logPrint("[-]\tReleasing of Rendering Context...");
		if (!wglDeleteContext(engGLRC)) {
			logPrint("FAILED.\n");
			engGLRC=NULL;
		} else
		logPrint("OK.\n");
	}
	logPrint("[-]\tReleasing of Device Context...");
	if (engDC && !ReleaseDC(engWnd,engDC)) {
		logPrint("FAILED.\n");
		engDC=NULL;
	} else
	logPrint("OK.\n");
	logPrint("[-]\tDestroying window...");
	if (engWnd && !DestroyWindow(engWnd)) {
		logPrint("FAILED.\n");
		engWnd=NULL;
	} else
	logPrint("OK.\n");

	logPrint("[-]\tUnregistring Window Class...");
	if (!UnregisterClass("nEngine",engInstance)) {
		logPrint("FAILED.\n");
		engInstance=NULL;
	} else
	logPrint("OK.\n");

	logPrint("[-]\t>>Closing engine<<\n");
	if (!usrSceneDone())
		logPrint("[!]\tFailed to close engine.\n");
	logClose();
}
void mainEngineExit(void) {
	PostQuitMessage(0);
}
unsigned char ComponentFromIndex(unsigned int i,UINT nb, UINT sh) {
	unsigned char trito8[8] = {0,36,73,109,146,182,219,255};
	unsigned char twoto8[4] = {0,85,170,255};
	unsigned char oneto8[2] = {0,255};
	switch(nb) {
		case 1: return oneto8[i&0x1];
		case 2: return twoto8[(i>>sh&0x3)];
		case 3: return trito8[(i>>sh&0x7)];
		default: return 0;
	}
}
void mainChangeVideoMode(int width, int height, int bits) {
		DEVMODE dmSS;
		memset(&dmSS,0,sizeof(dmSS));
		dmSS.dmSize=sizeof(dmSS);
		dmSS.dmPelsWidth	= engResX;
		dmSS.dmPelsHeight	= engResY;
		dmSS.dmBitsPerPel	= engBpp;
		dmSS.dmFields		= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		logPrint("[*]\tChanging display mode...");
		if (ChangeDisplaySettings(&dmSS,0)!=DISP_CHANGE_SUCCESSFUL) {
			logPrint("FAILED.\n[!]\tDisplay mode not supported, uses window.\n");
			engFScr=FALSE;
		} else logPrint("OK.\n");
}
void PASCAL OneTimer(UINT wTimerID, UINT msg,DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
	usrSceneChange(0);
}
bool mainEngineInit(char* title, int width, int height, int bits, bool fullscreen) {
	logOpen(LOGFILENAME);
	
	engBpp	= bits;
	engResX	= width;
	engResY	= height;
	engFScr	= fullscreen;

	logPrint("\t***Main Initializing***\n");
	
	
	unsigned int PixelFormat;

	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;

	WindowRect.left		= (long)0;
	WindowRect.right	= (long)engResX;
	WindowRect.top		= (long)0;
	WindowRect.bottom	= (long)engResY;

	logPrint("[*]\tDisplay mode:\n");
	logPrint("\t%ux%ux%ubpp, ",width,height,bits);

	if (fullscreen) 
		logPrint("fullscreen\n"); else
		logPrint("windowed\n");

	if (engFScr)
		mainChangeVideoMode(engResX, engResY, engBpp);
	
	if (engFScr) {
		ShowCursor(false);
		dwExStyle=WS_EX_APPWINDOW;
		dwStyle=WS_POPUP;
	} else {
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle=WS_OVERLAPPEDWINDOW;
	}
	
	engInstance			= GetModuleHandle(NULL);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) mainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= engInstance;
	wc.hIcon			= LoadIcon(engInstance, MAKEINTRESOURCE(IDI_ICON3));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "nEngine";
	
	logPrint("[*]\tRegistering Window Class...");
	if (!RegisterClass(&wc)){
		logPrint("FAILED.\n");
		return FALSE;		
	} else
	logPrint("OK.\n");



	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	logPrint("[*]\tCreating window...");
	if (!(engWnd=CreateWindowEx(dwExStyle,
								"nEngine",
								title,
								dwStyle |
								WS_CLIPSIBLINGS |
								WS_CLIPCHILDREN,
								0, 0,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,
								NULL,
								engInstance,
								NULL))) 
	{
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	} else
	logPrint("OK.\n");
	LOGPALETTE *pPal;
	pPal = (PLOGPALETTE)new char[sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY)];
	pPal->palVersion	= 0x300;
	pPal->palNumEntries	= 256;
	for(int i=0;i<256;i++) {
		pPal->palPalEntry[i].peRed		= ComponentFromIndex(i,3,0);
		pPal->palPalEntry[i].peGreen	= ComponentFromIndex(i,3,3);
		pPal->palPalEntry[i].peBlue		= ComponentFromIndex(i,2,6);
		pPal->palPalEntry[i].peFlags	= 0;
	}
	HPALETTE pal = CreatePalette(pPal);
	delete pPal;
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_NEED_PALETTE |
		PFD_SWAP_EXCHANGE,
		PFD_TYPE_RGBA,
		bits,
		3, 0,
		3, 3,
		2, 6,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		16,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	logPrint("[*]\tCreating GL Device Context...");
	if (!(engDC=GetDC(engWnd))) {
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	} else
	logPrint("OK.\n");


	logPrint("[*]\tChoosing pixel format...");
	if (!(PixelFormat=ChoosePixelFormat(engDC,&pfd))) {
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	} else
	logPrint("OK.\n");


	logPrint("[*]\tSetting pixel format...");
	if(!SetPixelFormat(engDC,PixelFormat,&pfd)) {
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	}
	logPrint("OK.\n");
	if (engBpp==8)
	{
		SelectPalette(engDC,pal,FALSE);
		RealizePalette(engDC);
	}

	logPrint("[*]\tCreating GL Rendering Context...");
	if (!(engGLRC=wglCreateContext(engDC))) {
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	}
	logPrint("OK.\n");
	logPrint("[*]\tActivating GL Rendering Context...");
	if(!wglMakeCurrent(engDC,engGLRC)) {
		logPrint("FAILED.\n");
		mainEngineShutdown();
		return FALSE;
	} else
	logPrint("OK.\n");

	ShowWindow(engWnd,SW_SHOW);
	SetForegroundWindow(engWnd);
	SetFocus(engWnd);
	logPrint("[*]\t<<Initializing engine>>\n");
	if (!usrSceneInit()) {
		logPrint("[!]\tEngine initializing failed.\n");
		mainEngineShutdown();
		return FALSE;
	} else
	logPrint("[+]\tEngine was initialized successfully.\n");
	usrSceneReSize(width, height);
	mainTimerID = timeSetEvent(10,0,OneTimer,NULL, TIME_PERIODIC);
	return TRUE;
}
void mainEngineLoop(void) {
	MSG msg;
	logPrint("[*]\tMain engine loop...\n");
	while ( GetMessage (&msg, NULL,0,0) )
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
/*	while(true){
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) 
			if (msg.message==WM_QUIT) break; else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 
	}*/
	logPrint("[-]\tMain engine loop was breaked.\n");
}
LRESULT CALLBACK mainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch (uMsg) {
		case WM_ACTIVATE: 
			if (!HIWORD(wParam)) 
			engActive=true;
			else engActive=false;
			return 0;
		case WM_SYSCOMMAND: 
			switch (wParam) {
				case SC_SCREENSAVE:;
				case SC_MONITORPOWER:;
				return 0;
			}
			break;
		case WM_CLOSE: 
			mainEngineExit();
			return 0;
		
		case WM_LBUTTONDOWN:inKeys[256] = true;usrMouseDown(0);break;
		case WM_RBUTTONDOWN:inKeys[257] = true;usrMouseDown(1);break;
		case WM_MBUTTONDOWN:inKeys[258] = true;usrMouseDown(2);break;

		case WM_LBUTTONUP:inKeys[256] = false;usrMouseUp(0);break;
		case WM_RBUTTONUP:inKeys[257] = false;usrMouseUp(1);break;
		case WM_MBUTTONUP:inKeys[258] = false;usrMouseUp(2);break;

		case WM_KEYDOWN: inKeys[wParam] = true;break;
		case WM_KEYUP: inKeys[wParam] = false;break;
		case WM_CHAR:
			conKeyProcess(char(wParam));
			break;
		case WM_SIZE: 
//			usrSceneReSize(LOWORD(lParam),HIWORD(lParam));
		break;
		case WM_PAINT:
			usrSceneReSize(engResX,engResY);
			usrSceneDraw();
			glDrawBuffer(GL_BACK_LEFT);
			SwapBuffers(engDC);
			FPSCount++;
			sprintf(FPSString,"FPS: %f",(float)FPSCount/(((float)GetTickCount()-(float)FPSLast)/1000));
			if (GetTickCount()-FPSLast>1000) {
				FPSLast=(float)GetTickCount();
				FPSCount=0;
			}
			return 0;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
#include "engUser.h"
#endif