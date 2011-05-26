#ifndef HDIRECTXCLASS
#define HDIRECTXCLASS

#include "directXHeader.h"
#include "gamecore.h"
#include "gamestate.h"

#include "humanPlayerClass.h"
#include "mainPlayerClass.h"

#include "AIPlayer.h"
#include "terrain.h"
#include "Networking\SocketExtras.h"
#include "Networking\Text.h"
#include "Networking\GameChat.h"
#include "Networking\MsgTranslator.h"
#include "Networking\Server.h"
#include "Networking\Client.h"
#include "Camera.h"
#include "projectile.h"
#include "gamestate.h"
#include "heavyShipClass.h"
#include "standardShipClass.h"
#include "lightShipClass.h"
#include "resource.h"
//#include "Networking/GameChat.h"
//using namespace std;

#define WINDOWED TRUE
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/*
	Used by ship constructor helper.
*/
static enum EShipType
{
	LIGHT = 0,
	STANDARD = 1,
	HEAVY = 2
};

/*
	Used by player constructor helper.
*/
static enum EPlayerType
{
	HUMAN = 0,
	AI = 1
};

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
	g_pMeshHeavy(NULL),
	g_pMeshMaterialsHeavy(NULL),
	g_pMeshTexturesHeavy(NULL),
	g_dwNumMaterialsHeavy(0L), 
	g_pMeshLight(NULL), 
	g_pMeshMaterialsLight(NULL),
	g_pMeshTexturesLight(NULL),
	g_dwNumMaterialsLight(0L), 
	g_pMeshLaser(NULL),
	g_pMeshMaterialsLaser(NULL),
	g_pMeshTexturesLaser(NULL), 
	g_dwNumMaterialsLaser(0L),  
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
	input(),
	camera(),
	singlePlayerSurface(0),
	singlePlayer(),
	multiPlayerSurface(0),
	multiPlayer(),
	healthSurface(0),
	healthRect(),
	radarSurface(0),
	radarRect(),
	bgSurface(0),
	menuSelect(0),
	dirLight(),
	dirLightEnabled(true),
	radarE(),
	radarA(){
      //Start Networking section
      _IamServer = false;
      _IamClient = false;
      _gameStarted = false;
      //End Networking section
   }
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow);
	static long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static directXClass *program;
	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	static void SetError(wchar_t* szFormat, ...);
	int GameInit();
	int GameLoop(float timeDelta);
	int GameShutdown();
	int LoadBitmapToSurface(wchar_t* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice);
	void PrintFrameRate( int x, int y, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	void directXClass::PrintString( int x, int y, char* String, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	void directXClass::PrintChar( int x, int y, char Character, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch );
	HRESULT LoadAlphabet( wchar_t* strPathName, int LetterWidth, int LetterHeight );
	int Render();
	int RenderMainMenu();
	int RenderRadar();
	int UpdateHUD();
	void FrameCount();
	HRESULT UnloadAlphabet();
	HRESULT directXClass::InitTiming();
	VOID SetupMatrices(bool mesh1Active);
	VOID Cleanup();
	HRESULT InitGeometry();
	static D3DXMATRIX Translate(const float dx, const float dy, const float dz);
	GameState _gamestate;
	static BOOL CALLBACK startDialog(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static MainPlayerClass shipBuilder(EShipType sType, EPlayerType pType, int pIdx, HWND hwnd, WCHAR wszBuff[256], WCHAR wszBuff2[1], WCHAR wszBuff3[2]);
    static MainPlayerClass* shipBuilder2(EShipType sType, EPlayerType pType, int pIdx, HWND hwnd, WCHAR wszBuff[256], WCHAR wszBuff2[1], WCHAR wszBuff3[2]);
    static Terrain getMainTerrain() {
	 	return mainTerrain;
    }
	//Start networking stuff
	Client* GetClient();
	Server* GetServer();
	Client _client;
	MsgTranslator _msgt;
	bool _IamClient;
	bool _IamServer;
	Server _server; 
	bool _gameStarted;
	//End networking stuff
private:
   //Start Networking stuff
   void StartAIs(); // Starts all AIs at the same time
   bool _iniframe; // synchronizes frames
   float _timeDelta;
   char _netmsg[3]; // Temporarily saves msgs to be sent by clients    
   char _clientID;
   void OnServerSocketEvent(LPARAM lParam, WPARAM wParam, HWND hWnd);
   void OnClientSocketEvent(LPARAM lParam, HWND hWnd);
   bool CreateServer(HWND hWnd);
   bool CreateClient(HWND hWnd, char *hostip);
   void ProcessMsg(Msg* msg, HWND hWnd);
   void ProcessMsc(Msg *msg);
   void ProcessTxt(Msg* msg);
   void ProcessClientCmd(Msg* msg, HWND hWnd);
   //End networking stuff
   static void IniPlayerLocation(MainPlayerClass* player, float px, float py, float pz, float rx, float ry, float rz);
	static void RespawnPlayer(MainPlayerClass* player);
	void drawLine(float startX, float startY, float endX, float endY, LPDIRECT3DSURFACE9 pBackSurf);
	void inputCommands(float timeDelta);
	void setupCubes();
	void cleanupCubes();
	void drawCubes();
	void updateCameraTarget();
    void IniChat();
	D3DLIGHT9 initDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	void setupLights();
    bool KeyDownChat(WPARAM wParam, HWND hWnd);
	point getMouseCoordinates();
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
	LPD3DXMESH              g_pMeshHeavy; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterialsHeavy; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTexturesHeavy; // Textures for our mesh
	DWORD                   g_dwNumMaterialsHeavy;   // Number of mesh materials
	LPD3DXMESH              g_pMeshLight; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterialsLight; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTexturesLight; // Textures for our mesh
	DWORD                   g_dwNumMaterialsLight;   // Number of mesh materials
	LPD3DXMESH              g_pMeshLaser; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterialsLaser; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTexturesLaser; // Textures for our mesh
	DWORD                   g_dwNumMaterialsLaser;   // Number of mesh materials
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
    //std::list<AIPlayer*> _aiPlayer;
    //AIPlayer _aiplayer1;
	Input input;
	ID3DXMesh *cubemesh[256];
	static int currentX;
	static int currentY;
	LPDIRECT3DSURFACE9 singlePlayerSurface;
	RECT singlePlayer; // rectangle for the single player image
	LPDIRECT3DSURFACE9 multiPlayerSurface;
	RECT multiPlayer; // rectangle for the multiplayer image
	LPDIRECT3DSURFACE9 healthSurface;
	RECT healthRect;
	LPDIRECT3DSURFACE9 radarSurface;
	RECT radarRect;
	LPDIRECT3DSURFACE9 bgSurface; //surface for working with the background
	int menuSelect; // int for what menu item is currently selected
    GameChat _chat;
    static Terrain mainTerrain; 
	boundingBox boundtemp;
	Camera camera;
	static float lastTime;
	D3DLIGHT9 dirLight;
	bool dirLightEnabled;
	MainShipClass dummyAI;
	MainShipClass dummyAI2;
	HWND hwndDialog;
	static bool waiting;
	MainPlayerClass *currentPlayers[8];
	LPDIRECT3DSURFACE9 radarE;
	LPDIRECT3DSURFACE9 radarA;
	RECT radarPlayers[8];
    //MsgTranslator _msgt;
};
#endif