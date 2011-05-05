#ifndef HDIRECTXCLASS
#define HDIRECTXCLASS
#include "mainPlayerClass.h"
#include "input.h"
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
	pSurface(0),bitmapName(bitmapNameNew),g_FrameCount(0),g_Frequency(0),
	g_pMesh(NULL),
	g_pMeshMaterials(NULL),
	g_pMeshTextures(NULL),
	g_dwNumMaterials(0L),
	g_pMesh2(NULL),
	g_pMeshMaterials2(NULL),
	g_pMeshTextures2(NULL),
	g_dwNumMaterials2(0L),
	rotationAboutYMesh1(0),
	rotationAboutXMesh1(0),
	rotationAboutYMesh2(0),
	rotationAboutXMesh2(0),
	rotationAboutYView(0),
	rotationAboutXView(0),
	translateZView(0),
	translateYMesh1(0),
	translateXMesh1(0),
	translateYMesh2(0),
	player1(),
	input(){}
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow);
	static long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static directXClass *program;
	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	static void SetError(wchar_t* szFormat, ...);
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
	VOID SetupMatrices(bool mesh1Active);
	VOID Cleanup();
	HRESULT InitGeometry();
	static D3DXMATRIX Translate(const float dx, const float dy, const float dz);
private:
	void drawLine(float startX, float startY, float endX, float endY, LPDIRECT3DSURFACE9 pBackSurf);
	void inputCommands();
	void setupCubes();
	void cleanupCubes();
	void drawCubes();
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
	LPD3DXMESH              g_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTextures; // Textures for our mesh
	DWORD                   g_dwNumMaterials;   // Number of mesh materials
	LPD3DXMESH              g_pMesh2; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterials2; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTextures2; // Textures for our mesh
	DWORD                   g_dwNumMaterials2;   // Number of mesh materials
	float rotationAboutYMesh1;
	float rotationAboutXMesh1;
	float rotationAboutYMesh2;
	float rotationAboutXMesh2;
	float rotationAboutYView;
	float rotationAboutXView;
	float translateZView;
	float translateYMesh1;
	float translateXMesh1;
	float translateYMesh2;
	float translateXMesh2;
	MainPlayerClass player1;
	Input input;
	ID3DXMesh *cubemesh[256];
};
#endif