#include "directXHeader.h"
#include "directXClass.h"

//main function, deals with everything
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	//filename of bitmap to display
	wchar_t * bitmapName = TEXT("kohakhisui1204.bmp");
	directXClass program = directXClass(bitmapName);
	directXClass::program = &program;
	program.WinMain(hInstance, hPrevInstance, pstrCmdLine, iCmdShow);
}


