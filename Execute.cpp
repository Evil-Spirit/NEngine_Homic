//NENGINE_EXECUTABLE
#include <windows.h>
#include "engMain.h"
#include "engResource.h"
#include <commctrl.h>
#pragma warning( disable : 4800 )
int modes [10][2] = 
{
	{320,200},
	{320,240},
	{400,300},
	{512,384},
	{640,400},
	{640,480},
	{800,600},
	{1024,768},
	{1280,1024},
	{1600,1200}
};
bool Exit = false;
BOOL CALLBACK DialogProc(HWND  dWnd, UINT  uMsg, WPARAM  wParam, LPARAM  lParam) {
	switch (uMsg) {
		case WM_CLOSE:
			EndDialog(dWnd,0);
			Exit=true;
			break;

		case WM_INITDIALOG:
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"320x200");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"320x240");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"400x300");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"512x384");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"640x400");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"640x480");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"800x600");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"1024x768");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"1280x1024");
			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_ADDSTRING, (long)0, (long)"1600x1200");
			SendDlgItemMessage(dWnd, IDC_COMBO_BPP , CB_ADDSTRING, (long)0, (long)"08 Bit");
			SendDlgItemMessage(dWnd, IDC_COMBO_BPP , CB_ADDSTRING, (long)0, (long)"16 Bit");
			SendDlgItemMessage(dWnd, IDC_COMBO_BPP , CB_ADDSTRING, (long)0, (long)"32 Bit");

			SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_SETCURSEL, (long)5, (long)0);
			SendDlgItemMessage(dWnd, IDC_COMBO_BPP , CB_SETCURSEL, (long)1, (long)0);

			SendDlgItemMessage(dWnd, IDC_CHECK_FULL,BM_SETCHECK, (long)1, (long)0);
			SendDlgItemMessage(dWnd, IDC_CHECK_MIP ,BM_SETCHECK, (long)1, (long)0);
			SendDlgItemMessage(dWnd, IDC_CHECK_BIL ,BM_SETCHECK, (long)1, (long)0);
			SendDlgItemMessage(dWnd, IDC_CHECK_TRI ,BM_SETCHECK, (long)1, (long)0);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					engMip	= (bool)SendDlgItemMessage(dWnd, IDC_CHECK_MIP ,BM_GETCHECK, (long)0, (long)0);
					engBil	= (bool)SendDlgItemMessage(dWnd, IDC_CHECK_BIL ,BM_GETCHECK, (long)0, (long)0);
					engFScr	= (bool)SendDlgItemMessage(dWnd, IDC_CHECK_FULL,BM_GETCHECK, (long)0, (long)0);
					int tril;
					tril =
						SendDlgItemMessage(dWnd, IDC_CHECK_TRI,BM_GETCHECK, (long)0, (long)0);
					if (tril == 2) engTri = false; else engTri = (bool)tril;
					tril =
						SendDlgItemMessage(dWnd, IDC_COMBO_MODE, CB_GETCURSEL, (long)0, (long)0);
					engResX	= modes[tril][0];
					engResY	= modes[tril][1];

					tril =
						SendDlgItemMessage(dWnd, IDC_COMBO_BPP, CB_GETCURSEL, (long)0, (long)0);
					engBpp	= (tril+1)*8;
					if (engBpp==24) engBpp = 32; 
					EndDialog(dWnd,1);
					break;
				case IDC_CHECK_MIP:
					if(!SendDlgItemMessage(dWnd, IDC_CHECK_MIP,BM_GETCHECK, (long)0, (long)0))
					{
						SendDlgItemMessage(dWnd, IDC_CHECK_TRI,BM_SETSTYLE, (long)BS_3STATE, (long)1);
						SendDlgItemMessage(dWnd, IDC_CHECK_TRI,BM_SETCHECK, (long)2, (long)0);
					} else
					{
						SendDlgItemMessage(dWnd, IDC_CHECK_TRI,BM_SETSTYLE, (long)BS_AUTOCHECKBOX, (long)1);
						SendDlgItemMessage(dWnd, IDC_CHECK_TRI,BM_SETCHECK, (long)0, (long)0);
					}
					break;
				case IDCANCEL:
					EndDialog(dWnd,0);
					Exit = true;
					break;
				default : break;
			}
		break;

		case WM_DESTROY:
			break;
		default : break;
	}
	return 0;
}
BOOL CALLBACK DialogProc1(HWND  dWnd, UINT  uMsg, WPARAM  wParam, LPARAM  lParam) {
	switch (uMsg) {
		case WM_CLOSE:
			EndDialog(dWnd,0);
			Exit=true;
			break;
		case WM_INITDIALOG:
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(dWnd,1);
					break;
				case IDCANCEL:
					EndDialog(dWnd,0);
					Exit = true;
					break;
				default : break;
			}
		break;
		case WM_DESTROY:
			break;
		default : break;
	}
	return 0;
}
int WINAPI WinMain (HINSTANCE hThisInst,HINSTANCE hPrevInst,
                    LPSTR lpszArgs,int nWinMode) 
{
	InitCommonControls();
	DialogBox(	NULL,
				MAKEINTRESOURCE(IDD_INFO),
				NULL,
				(DLGPROC)DialogProc1);
	if(!Exit) 
	DialogBox(	NULL,
				MAKEINTRESOURCE(IDD_CHOICE_GRAPHIC),
				NULL,
				(DLGPROC)DialogProc);
	if(!Exit) 
	{ 
		mainEngineInit("Evil Spirit and Drive OpenGL Engine",engResX,engResY,engBpp,engFScr);
		mainEngineLoop();
		mainEngineShutdown();
	}
	return 0;
}