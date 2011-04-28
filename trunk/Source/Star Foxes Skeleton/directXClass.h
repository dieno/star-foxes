#ifndef HDIRECTXCLASS
#define HDIRECTXCLASS
#include "directXHeader.h"


//main class used by the program for everything
class directXClass {
public:
	directXClass(wchar_t* bitmapNameNew):g_FrameRate(0), 
	g_AlphabetLetterWidth(0),
	g_bAlphabetLoaded(FALSE),
	g_AlphabetLettersPerRow(0),
	g_AlphabetLetterHeight(0),
	g_pAlphabetSurface(0),
	g_AlphabetWidth(0),
	g_AlphabetHeight(0),
	pSurface(0),bitmapName(bitmapNameNew),g_FrameCount(0),g_Frequency(0){}
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow);
	static long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static directXClass *program;
	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	void SetError(wchar_t* szFormat, ...);
	int GameInit();
	int GameLoop();
	int GameShutdown();
	int LoadBitmapToSurface(wchar_t* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice);
	void PrintFrameRate( int x, int y, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	void directXClass::PrintString( int x, int y, char* String, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	void directXClass::PrintChar( int x, int y, char Character, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	HRESULT LoadAlphabet( wchar_t* strPathName, int LetterWidth, int LetterHeight );
	int Render();
	void FrameCount();
	HRESULT UnloadAlphabet();
	HRESULT directXClass::InitTiming();
private:
	void drawLine(float startX, float startY, float endX, float endY, LPDIRECT3DSURFACE9 pBackSurf);
	static LPDIRECT3D9 g_pD3D;//COM object
	static LPDIRECT3DDEVICE9 g_pDevice;//graphics device
	static HWND g_hWndMain;//handle to main window
	static float startX, startY, endX, endY;
	static bool startMouseMove;
	static int height, width;
	INT64 g_FrameRate;
	int g_AlphabetLetterWidth;
	BOOL g_bAlphabetLoaded;
	int g_AlphabetLettersPerRow;
	int g_AlphabetLetterHeight;
	LPDIRECT3DSURFACE9 g_pAlphabetSurface;
	int g_AlphabetWidth;
	int g_AlphabetHeight;
	LPDIRECT3DSURFACE9 pSurface;
	wchar_t* bitmapName;
	INT64 g_FrameCount;
	INT64 g_Frequency;
};
#endif