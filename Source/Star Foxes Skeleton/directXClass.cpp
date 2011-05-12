#include "directXClass.h"

LPDIRECT3D9 directXClass::g_pD3D = 0;
LPDIRECT3DDEVICE9 directXClass::g_pDevice = 0;
HWND directXClass::g_hWndMain;
directXClass *directXClass::program = 0;
float directXClass::startX = 0;
float directXClass::startY = 0;
float directXClass::endX = 0;
float directXClass::endY = 0;
int directXClass::height = 680;
int directXClass::width = 480;
bool directXClass::startMouseMove = false;
int directXClass::currentX;
int directXClass::currentY;

//deals with revaildating the window, and the basic window stuff
long CALLBACK directXClass::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam){
	switch(uMessage){
	case WM_SIZE:
		{
			return 0;
		}
	case WM_CREATE:
		{
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			currentX = LOWORD(lParam);
			currentY = HIWORD(lParam);
			return 0;
		}
	case WM_PAINT:
		{
			ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
   case WM_KEYDOWN:
      program->KeyDownChat(wParam, hWnd);
      return 0;
	default:
		{
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
	}
}

//creates the window, and runs the gameloop/message loop
int WINAPI directXClass::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow){
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;
	HRESULT r=0;

	wchar_t strAppName[] = TEXT("First Windows App, Zen Style");

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = directXClass::WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = strAppName;

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		strAppName,
		strAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	g_hWndMain = hWnd;//set our global window handle
	
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	
	if(FAILED(GameInit())){;//initialize Game
		SetError(TEXT("Initialization Failed"));
		GameShutdown();
		return E_FAIL;
	}
	
	r=LoadBitmapToSurface(bitmapName, &pSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load background bitmap surface"));
	}

	r=LoadBitmapToSurface(TEXT("stars.bmp"), &bgSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load main menu background surface"));
	}

	singlePlayerSurface = 0;

	singlePlayer.bottom = 412;
	singlePlayer.left = 240;
	singlePlayer.right = 400;
	singlePlayer.top = 368;

	r=LoadBitmapToSurface(TEXT("SinglePlayer.bmp"), &singlePlayerSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load singleplayer menu item bitmap surface"));
	}

	multiPlayerSurface = 0;

	multiPlayer.bottom = 440;
	multiPlayer.left = 270;
	multiPlayer.right = 370;
	multiPlayer.top = 420;

	r=LoadBitmapToSurface(TEXT("MultiPlayer.bmp"), &multiPlayerSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load multiplayer menu item bitmap surface"));
	}

	healthSurface = 0;

	healthRect.bottom = 460;
	healthRect.left = 60;
	healthRect.right = 460;
	healthRect.top = 420;

	r=LoadBitmapToSurface(TEXT("health.bmp"), &healthSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load multiplayer menu item bitmap surface"));
	}

	radarSurface = 0;

	radarRect.bottom = 460;
	radarRect.left = 480;
	radarRect.right = 600;
	radarRect.top = 340;

	r=LoadBitmapToSurface(TEXT("radar.bmp"), &radarSurface, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load multiplayer menu item bitmap surface"));
	}

	while(TRUE){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			GameLoop();
		}
	}
	GameShutdown();// clean up the game
	return msg.wParam;
}

//initializes the direct 3D device
int directXClass::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice){
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r=0;

	if(*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(r)){
		SetError(TEXT("Could not get display adapter information"));
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	
	r=pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if(FAILED(r)){
		SetError(TEXT("Could not create the render device"));
		return E_FAIL;
	}
	
    // Turn on the zbuffer
    g_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // Turn on ambient lighting 
    g_pDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
//	g_DeviceHeight = Height;
//	g_DeviceWidth = Width;

//	g_SavedPresParams = d3dpp;
	return S_OK;
}


//error function, displays errors on console
void directXClass::SetError(wchar_t* szFormat, ...){
	wchar_t szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsnwprintf_s(szBuffer, sizeof(szBuffer)/sizeof(char), szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString(TEXT("\n"));
	printf("error!!");
}


//initializes the game
int directXClass::GameInit(){
	HRESULT r = 0;//return values
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if( g_pD3D == NULL){
		SetError(TEXT("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
	
	r = InitDirect3DDevice(g_hWndMain, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOWED, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	InitGeometry();
	LoadAlphabet(TEXT("Alphabet vSmall.bmp"), 8, 16);
	InitTiming();
	MainShipClass dummyAI = MainShipClass(g_pMesh, g_pMeshMaterials, g_pMeshTextures, g_dwNumMaterials, g_pDevice);
	dummyAI.setTranslateZ(-3);
	dummyAI.setRotationAboutY(D3DX_PI);
	player1 = HumanPlayerClass(g_pMesh, g_pMeshMaterials, g_pMeshTextures, g_dwNumMaterials, g_pDevice, "Human",0, 1);

   static D3DMATERIAL9* mat1 = new D3DMATERIAL9;
   static AIPlayer ai1 = AIPlayer(g_pMesh, mat1, g_pMeshTextures, 1, g_pDevice, "Human",0, 1);
   ai1.GetShip()->setTranslation(100, 100, -5);
   ai1.GetShip()->setRotation(0, 3.1f, 0);   
   ai1.SetBounds(ai1.getPosition());   
   ai1.GetShip()->SetMtrlColor(D3DXCOLOR(0, 255.0f, 0, 255.0f), D3DXCOLOR(0, 255.0f, 0, 255.0f), D3DXCOLOR(0, 255.0f, 0, 255.0f));
   
   static D3DMATERIAL9* mat2 = new D3DMATERIAL9;
   static AIPlayer ai2 = AIPlayer(g_pMesh, mat2, g_pMeshTextures, 1, g_pDevice, "Human",0, 1);
   ai2.GetShip()->setTranslation(100, 100, -5);
   ai2.GetShip()->setRotation(0, 3.1f, 0);
   ai2.SetBounds(ai2.getPosition());
   ai2.GetShip()->SetMtrlColor(D3DXCOLOR(0, 255.0f, 0, 255.0f), D3DXCOLOR(0, 255.0f, 0, 255.0f), D3DXCOLOR(0, 255.0f, 0, 255.0f));

   static D3DMATERIAL9* mat3 = new D3DMATERIAL9;
   static AIPlayer ai3 = AIPlayer(g_pMesh, mat3, g_pMeshTextures, 1, g_pDevice, "Human",0, 1);
   ai3.GetShip()->setTranslation(100, 100, -5);
   ai3.GetShip()->setRotation(0, 3.1f, 0);
   ai3.SetBounds(ai3.getPosition());
   ai3.SetBehaviour(FLEE);
   ai3.GetShip()->SetMtrlColor(D3DXCOLOR(0, 0, 255.0f, 255.0f), D3DXCOLOR(0, 0, 255.0f, 255.0f), D3DXCOLOR(0, 0, 255.0f, 255.0f));

   static D3DMATERIAL9* mat4 = new D3DMATERIAL9;
   static AIPlayer ai4 = AIPlayer(g_pMesh, mat4, g_pMeshTextures, 1, g_pDevice, "Human",0, 1);
   ai4.GetShip()->setTranslation(100, 100, -5);
   ai4.GetShip()->setRotation(0, 3.1f, 0);
   ai4.SetBounds(ai4.getPosition());
   ai4.GetShip()->SetMtrlColor(D3DXCOLOR(255.0f, 0, 0, 255.0f), D3DXCOLOR(255.0f, 0, 0, 255.0f), D3DXCOLOR(255.0f, 0, 0, 255.0f));
   ai4.SetBehaviour(SEEK);


   //static AIPlayer aiplayer = AIPlayer(g_pMesh, g_pMeshMaterials, g_pMeshTextures, g_dwNumMaterials, g_pDevice, "Human",0, 1);
   _aiPlayer.push_back(&ai3);
   _aiPlayer.push_back(&ai1);
   _aiPlayer.push_back(&ai2);
   _aiPlayer.push_back(&ai4);
	//player2 = MainPlayerClass("Dummy",0, 1, dummyAI);
   
	if(FAILED(r)){//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(TEXT("Initialization of the device failed"));
		return E_FAIL;
	}

	input.init_keyboard(g_hWndMain);
	setupCubes();

	return S_OK;
}

//the game loop, renders, counts frames, and quits on esc key down
int directXClass::GameLoop(){
	FrameCount();
	input.read_keyboard();
	
	// Main menu switch, determines whether to go into single or multiplayer
	switch(menuSelect)
	{
		// Singleplayer menu item is selected
		case 0:
			// Selects multiplayer menu item if down is pressed
			if (input.get_keystate(DIK_DOWN))
			{
				menuSelect = 1;
				
				multiPlayer.bottom = 452;
				multiPlayer.left = 240;
				multiPlayer.right = 400;
				multiPlayer.top = 408;

				singlePlayer.bottom = 400;
				singlePlayer.left = 270;
				singlePlayer.right = 370;
				singlePlayer.top = 380;

			}
			// Selects multiplayer menu item if within its rectangle
			if (currentX < multiPlayer.right && currentX > multiPlayer.left && 
				currentY > multiPlayer.top && currentY < multiPlayer.bottom)
			{
				menuSelect = 1;

				multiPlayer.bottom = 452;
				multiPlayer.left = 240;
				multiPlayer.right = 400;
				multiPlayer.top = 408;

				singlePlayer.bottom = 400;
				singlePlayer.left = 270;
				singlePlayer.right = 370;
				singlePlayer.top = 380;

			}
			// If enter is pressed the program enters a singleplayer game
			if (input.get_keystate(DIK_RETURN))
			{
				menuSelect = 2;
			}
			RenderMainMenu();
		break;

		// Multiplayer menu item is selected
		case 1:
			// Selects singleplayer menu item if up is pressed
			if (input.get_keystate(DIK_UP))
			{

				menuSelect = 0;

				singlePlayer.bottom = 412;
				singlePlayer.left = 240;
				singlePlayer.right = 400;
				singlePlayer.top = 368;
				
				multiPlayer.bottom = 440;
				multiPlayer.left = 270;
				multiPlayer.right = 370;
				multiPlayer.top = 420;
				
			}
			// Selects singleplayer menu item if within its rectangle
			if (currentX < singlePlayer.right && currentX > singlePlayer.left && 
				currentY > singlePlayer.top && currentY < singlePlayer.bottom)
			{
				menuSelect = 0;

				singlePlayer.bottom = 412;
				singlePlayer.left = 240;
				singlePlayer.right = 400;
				singlePlayer.top = 368;
				
				multiPlayer.bottom = 440;
				multiPlayer.left = 270;
				multiPlayer.right = 370;
				multiPlayer.top = 420;
				
			}
			// If enter is pressed the program enters a multiplayer game
			if (input.get_keystate(DIK_RETURN))
			{
				menuSelect = 3;
			}
			RenderMainMenu();
		break;

		// In Singleplayer game
		case 2:
			inputCommands();
			player1.updateRotation();
			player1.updatePosition();
         //directXClass::SetError(TEXT("p1: %f"), player1.getPositionZ());
         for (std::list<AIPlayer*>::const_iterator ci = _aiPlayer.begin(); ci != _aiPlayer.end(); ++ci)
         {
           //directXClass::SetError(TEXT("p1: %f"), player1.getRotation().z); 
            //(*ci)->bankLeft(0.01f);
            //MessageBoxA(g_hWndMain, "hi", "hilo", 0);
            (*ci)->Update(g_hWndMain, player1.getPosition());
            //(*ci)->bankUp(0.04f);
            (*ci)->updateRotation();
            (*ci)->updatePosition();
            
         }
         //_aiPlayer.front()->bankLeft(0.01f);
         //_aiplayer1.bankLeft(0.01f);
         //player2.(0.01f);
			Render();
		break;

		// In Multiplayer game, currently the same as single player as there
		// is currently no difference
		case 3:
			inputCommands();
			player1.updateRotation();
			player1.updatePosition();
			Render();
		break;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

//runs when game ends, cleans up everything used by the game
int directXClass::GameShutdown(){
	cleanupCubes();
	
	UnloadAlphabet();
	input.clean_input();

	//release resources. First display adapter because COM object created it, then COM object
	if(g_pDevice)
		g_pDevice->Release();

	if(g_pD3D)
		g_pD3D->Release();

	return S_OK;
}

//renders the bitmap
int directXClass::Render(){
	HRESULT r;
	LPDIRECT3DSURFACE9 pBackSurf = 0;
	if(!g_pDevice){
		SetError(TEXT("Cannot render because there is no device"));
		return E_FAIL;
	}
	//clear the display arera with colour black, ignore stencil buffer
	g_pDevice->Clear(0,0,D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,25), 1.0f, 0);
    // Clear the backbuffer and the zbuffer
   g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	D3DLOCKED_RECT Locked;
	//get pointer to backbuffer
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer"));
	}
	
	

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface"));
	pBackSurf->LockRect(&Locked, 0, 0);
	PrintFrameRate(0, 0, TRUE, D3DCOLOR_ARGB(255,255,0,255), (DWORD*)Locked.pBits, Locked.Pitch);
	pBackSurf->UnlockRect();
	
	

	pBackSurf->Release();//release lock
	
	
	pBackSurf = 0;
	// Begin the scene
	if( SUCCEEDED( g_pDevice->BeginScene() ) )
	{
		// Setup the world, view, and projection matrices
		SetupMatrices(true);

		

		player1.drawSelf();
		//player2.drawSelf();
      for (std::list<AIPlayer*>::const_iterator ci = _aiPlayer.begin(); ci != _aiPlayer.end(); ++ci)
      {
         (*ci)->drawSelf();
      }     

		SetupMatrices(true);

		drawCubes();
        _chat.RenderChat();
		
		// End the scene
		g_pDevice->EndScene();
	}
	
	//get pointer to backbuffer for HUD
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer"));
	}

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &healthRect, healthSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface health"));

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &radarRect, radarSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface health"));

	pBackSurf->Release();//release lock
	
	pBackSurf = 0;

	g_pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

int directXClass::RenderMainMenu(){
	HRESULT r;
	LPDIRECT3DSURFACE9 pBackSurf = 0;
	

	if(!g_pDevice){
		SetError(TEXT("Cannot render because there is no device"));
		return E_FAIL;
	}
	//clear the display arera with colour black, ignore stencil buffer
	g_pDevice->Clear(0,0,D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,25), 1.0f, 0);
    // Clear the backbuffer and the zbuffer
    g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	D3DLOCKED_RECT Locked;
	//get pointer to backbuffer
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer"));
	}
	
	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, NULL, bgSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface main menu background"));

	pBackSurf->LockRect(&Locked, 0, 0);
	PrintFrameRate(0, 0, TRUE, D3DCOLOR_ARGB(255,255,0,255), (DWORD*)Locked.pBits, Locked.Pitch);
	pBackSurf->UnlockRect();

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &singlePlayer, singlePlayerSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface single player"));

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &multiPlayer, multiPlayerSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface multi player"));

	pBackSurf->Release();//release lock
	
	pBackSurf = 0;
	g_pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

//loads the bitmap onto a surface
int directXClass::LoadBitmapToSurface(wchar_t* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice){
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if(hBitmap == NULL){
		SetError(TEXT("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

	//create surface for bitmap
	r =	pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth,Bitmap.bmHeight,D3DFMT_X8R8G8B8,D3DPOOL_SCRATCH,ppSurface,NULL);

	if(FAILED(r)){
		SetError(TEXT("Unable to create surface for bitmap load"));
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
	if(FAILED(r)){
		SetError(TEXT("Unable to load file to surface"));
		return E_FAIL;
	}

	return S_OK;
}

//prints the framerate on a surface
void directXClass::PrintFrameRate( int x, int y, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch )
{
	char string[10];	// String to hold the frame rate
	
	// Zero out the string
	ZeroMemory( &string, sizeof( string ) );

	// Convert the frame rate to a string
	_itoa_s( (int)g_FrameRate, string, 10 );

	// Output the string to the back surface
	PrintString( x, y, string, TRUE, D3DCOLOR_ARGB( 255,255, 0, 255 ), pDestData, DestPitch );
}

//prints a string somewhere
void directXClass::PrintString( int x, int y, char* String, BOOL bTransparent, 
					D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch )
{
	// Loop for each character in the string
	for( UINT i = 0 ; i < strlen( String ) ; i++ )
	{
		// Print the current character
		PrintChar( x + (g_AlphabetLetterWidth * i), y, String[i], 
							bTransparent, ColorKey, pDestData, DestPitch );
	}	
}

//prints a char
void directXClass::PrintChar( int x, int y, char Character, BOOL bTransparent, 
						D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch )
{
	HRESULT r = 0;
	
	div_t Result;	// Holds the result of divisions

	// The offset into the alphabet image
	int OffsetX = 0, OffsetY = 0;

	POINT LetterDestPoint = { 0, 0 };	// The destination point for the letter
	RECT LetterRect = { 0, 0, 0, 0 };	// The source rectangle for the letter

	// If the alphabet has not been loaded yet then exit
	if( !g_bAlphabetLoaded )
		return;

	// The characters are specified in ASCII code, which begins at 32 so
	// we want to decrement this value by 32 to make it zero based
	Character -= 32;

	// Avoid divide by 0 errors
	if( Character == 0 )
		return;

	// Divide the character code by the number of letters per row.
	// The quotient will help get the vertical offset and the
	// remainder will help get the horizontal offset
	Result = div( Character, g_AlphabetLettersPerRow );

	// Get the horizontal offset by multiplying the remainder
	// by the width of the Letter
	OffsetX = Result.rem * g_AlphabetLetterWidth;
	// Get the vertical offset by multiplying the quotient
	// by the height of the letter
	OffsetY = Result.quot * g_AlphabetLetterHeight;

	// Fill in the source rectangle with the computed offsets
	SetRect( &LetterRect, OffsetX, OffsetY, 
	OffsetX + g_AlphabetLetterWidth, OffsetY + g_AlphabetLetterHeight );
	
	// Fill in the destination point
	LetterDestPoint.x = x;
	LetterDestPoint.y = y;
	
	D3DLOCKED_RECT LockedAlphabet;	// Holds info about the alphabet surface

	// Lock the source surface
	r = g_pAlphabetSurface->LockRect( &LockedAlphabet, 0, D3DLOCK_READONLY  );
	if( FAILED( r ) )
	{
		SetError(TEXT("Couldnt lock alphabet surface for PrintChar()"));
		return;
	}
	
	// Get a DWORD pointer to each surface
	DWORD* pAlphaData = (DWORD*)LockedAlphabet.pBits;

	// Convert the BYTE pitch pointer to a DWORD ptr
	LockedAlphabet.Pitch /=4;
	DestPitch /= 4;

	// Compute the offset into the alphabet
	int AlphaOffset = OffsetY * LockedAlphabet.Pitch + OffsetX;
	// Compute the offset into the destination surface
	int DestOffset = y * DestPitch + x;

	// Loop for each row in the letter
	for( int cy = 0 ; cy < g_AlphabetLetterHeight ; cy++ )
	{
		// Loop for each column in the letter
		for( int cx = 0 ; cx < g_AlphabetLetterWidth ; cx++ )
		{
			if( bTransparent )
			{
				// If this alphabet pixel is not transparent
				if( pAlphaData[ AlphaOffset ] != ColorKey )
				{
					// Then copy the pixel to the destination
					pDestData[ DestOffset ] = pAlphaData[ AlphaOffset ];
				}

				// Increment the offsets to the next pixel
				AlphaOffset++;
				DestOffset++;
			}
			else
				pDestData[ DestOffset ] = pAlphaData[ AlphaOffset ];
		}

		// Move the offsets to the start of the next row
		DestOffset += DestPitch - g_AlphabetLetterWidth;
		AlphaOffset += LockedAlphabet.Pitch - g_AlphabetLetterWidth;
	}
	
	// Unlock the surface
	g_pAlphabetSurface->UnlockRect();
	
}

//loads an aplhabet for the program to use for the chars it displays
HRESULT directXClass::LoadAlphabet( wchar_t* strPathName, int LetterWidth, int LetterHeight )
{
	// Make sure a valid path was specified
	if( !strPathName )
		return E_FAIL;

	// Make sure the size of the letters is greater than 0
	if( !LetterWidth || !LetterHeight )
		return E_FAIL;

	HRESULT r = 0;
	
	// Load the bitmap into memory
	r = LoadBitmapToSurface( strPathName, &g_pAlphabetSurface, g_pDevice );
	if( FAILED( r ) )
	{
		SetError(TEXT("Unable to load alphabet bitmap"));
		return E_FAIL;
	}

	// Holds information about the alpahbet surface
	D3DSURFACE_DESC d3dsd;

	// Get information about the alphabet surface
	g_pAlphabetSurface->GetDesc( &d3dsd );

	// Update globals with the letter dimensions
	g_AlphabetWidth = d3dsd.Width;			
	g_AlphabetHeight = d3dsd.Height;
	g_AlphabetLetterWidth = LetterWidth;
	g_AlphabetLetterHeight = LetterHeight;

	// Compute the number of letters in a row
	g_AlphabetLettersPerRow = g_AlphabetWidth / g_AlphabetLetterWidth;

	// Set the loaded flag to TRUE
	g_bAlphabetLoaded = TRUE;

	return S_OK;
}

//counts the frames to decide the framerate
void directXClass::FrameCount(){
	INT64 NewCount = 0;
	static INT64 LastCount = 0;
	INT64 Difference = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&NewCount);

	if( NewCount == 0)
		SetError(TEXT("The system does not support high resolution timing"));

	g_FrameCount++;

	Difference = NewCount - LastCount;

	if(Difference >= g_Frequency){
		g_FrameRate = g_FrameCount;
		g_FrameCount = 0;

		LastCount = NewCount;
	}
}

// Unloads the alphabet from memory
HRESULT directXClass::UnloadAlphabet()
{
	// Check if the alphabet exists
	if( g_pAlphabetSurface )
	{
		// Release the surface
		g_pAlphabetSurface->Release();
		// NULL the pointer
		g_pAlphabetSurface = 0;
		// Set the loaded flag to FALSE
		g_bAlphabetLoaded = FALSE;
	}

	return S_OK;
}

//initializes the frames per second counter
HRESULT directXClass::InitTiming(){
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency);

	if(g_Frequency == 0){
		SetError(TEXT("The system does not support high resolution timing"));
		return E_FAIL;
	}

	return S_OK;
}

//draws a line from start point, to end point
void directXClass::drawLine(float startX, float startY, float endX, float endY, LPDIRECT3DSURFACE9 pSurf) {
	D3DLOCKED_RECT Locked;
	pSurf->LockRect(&Locked,0,0);
	DWORD* pData = (DWORD*)(Locked.pBits);
	for (float t = 0; t < 1; t += 0.001f) {
		int X = (int)(startX + t * (endX - startX));
        int Y = (int)(startY + t * (endY - startY));
		DWORD Offset = (Y*Locked.Pitch/4 + X);
		pData[Offset] = D3DCOLOR_XRGB(0,0,255);
	}
	pSurf->UnlockRect();
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID directXClass::SetupMatrices(bool mesh1Active)
{
    // For our world matrix, we will just leave it as the identity
    D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 translate;
    D3DXMATRIXA16 matWorld2;
    D3DXMATRIXA16 translate2;
	if (mesh1Active == true) {

		D3DXMATRIX scale = Translate(0, 0, 0);
		D3DXMatrixRotationY( &rotationY, rotationAboutYMesh1 );
		D3DXMatrixRotationX( &rotationX, rotationAboutXMesh1 );
		translate = Translate(translateXMesh1, translateYMesh1, 0);
		D3DXMatrixMultiply(&translate2, &scale, &translate);
	} else {

		D3DXMATRIX scale = Translate(0, 0, 0);
		scale(0,0) = 0.25f;
		scale(1,1) = 0.25f;
		scale(2,2) = 0.25f;
		D3DXMatrixRotationY( &rotationY, rotationAboutYMesh2 );
		D3DXMatrixRotationX( &rotationX, rotationAboutXMesh2 );
		translate = Translate(translateXMesh2-1.25f, translateYMesh2, 0);
		D3DXMatrixMultiply(&translate2, &scale, &translate);
	}
	D3DXMatrixMultiply(&matWorld2, &rotationY, &rotationX);
	D3DXMatrixMultiply(&matWorld, &matWorld2, &translate2);
    g_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	
    D3DXVECTOR3 vEyePt( 0.0f, 3.0f, 5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	
    D3DXMATRIXA16 matView2;
	D3DXMatrixLookAtLH(&matView2, &vEyePt, &vLookatPt, &vUpVec);

	
    /*D3DXVECTOR3 vEyePt2( 0.0f, 3.0f,-5.0f );
    D3DXVECTOR3 vLookatPt2( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec2( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3 vRightVec2( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vLookVec = vEyePt - vLookatPt;
	camera.setLookAt(&vLookatPt2);
	camera.setPosition(&vEyePt2);
	camera.setUp(&vUpVec2);
	camera.setRight(&vRightVec2);
	camera.walk(translateZView);
	camera.strafe(rotationAboutXView);
	camera.fly(rotationAboutYView);
    D3DXMATRIXA16 matView;
	camera.getViewMatrix(&matView);*/
    g_pDevice->SetTransform( D3DTS_VIEW, &matView2 );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    g_pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Load the mesh and build the material and texture arrays
//-----------------------------------------------------------------------------
HRESULT directXClass::InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    if( FAILED( D3DXLoadMeshFromX( TEXT("airwing.x"), D3DXMESH_SYSTEMMEM, 
                                   g_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, 
                                   &g_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( TEXT("..\\airwing.x"), D3DXMESH_SYSTEMMEM, 
                                    g_pDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, 
                                    &g_pMesh ) ) )
        {
            MessageBox(NULL, TEXT("Could not find airwing.x"), TEXT("Meshes.exe"), MB_OK);
            return E_FAIL;
        }
    }
    
    // CHAT: Initialize chat
    IniChat();

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
    g_pMeshTextures  = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];

    for( DWORD i=0; i<g_dwNumMaterials; i++ )
    {
        // Copy the material
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen((LPCWSTR)d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
			int len = lstrlen((LPCWSTR)d3dxMaterials[i].pTextureFilename);
			wchar_t *wText = new wchar_t[len];
			::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterials[i].pTextureFilename, -1, wText,len );
            if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                wText, 
                                                &g_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const TCHAR* strPrefix = TEXT("..\\");
                const int lenPrefix = lstrlen( strPrefix );
                TCHAR strTexture[MAX_PATH];
                lstrcpyn( strTexture, strPrefix, MAX_PATH );
                lstrcpyn( strTexture + lenPrefix, wText, MAX_PATH - lenPrefix );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                    strTexture, 
                                                    &g_pMeshTextures[i] ) ) )
                {
                    MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

   // if( FAILED( D3DXLoadMeshFromX( TEXT("M_DISK.x"), D3DXMESH_SYSTEMMEM, 
   //                                g_pDevice, NULL, 
   //                                &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials2, 
   //                                &g_pMesh2 ) ) )
   // {
   //     // If model is not in current folder, try parent folder
   //     if( FAILED( D3DXLoadMeshFromX( TEXT("..\\M_DISK.x"), D3DXMESH_SYSTEMMEM, 
   //                                 g_pDevice, NULL, 
   //                                 &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials2, 
   //                                 &g_pMesh2 ) ) )
   //     {
   //         MessageBox(NULL, TEXT("Could not find M_DISK.x"), TEXT("Meshes.exe"), MB_OK);
   //         return E_FAIL;
   //     }
   // }

   // // We need to extract the material properties and texture names from the 
   // // pD3DXMtrlBuffer
   // D3DXMATERIAL* d3dxMaterials2 = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
   // g_pMeshMaterials2 = new D3DMATERIAL9[g_dwNumMaterials2];
   // g_pMeshTextures2  = new LPDIRECT3DTEXTURE9[g_dwNumMaterials2];

   // for( DWORD i=0; i<g_dwNumMaterials2; i++ )
   // {
   //     // Copy the material
   //     g_pMeshMaterials2[i] = d3dxMaterials2[i].MatD3D;

   //     // Set the ambient color for the material (D3DX does not do this)
   //     g_pMeshMaterials2[i].Ambient = g_pMeshMaterials2[i].Diffuse;

   //     g_pMeshTextures2[i] = NULL;
   //     if( d3dxMaterials2[i].pTextureFilename != NULL && 
   //         lstrlen((LPCWSTR)d3dxMaterials2[i].pTextureFilename) > 0 )
   //     {
   //         // Create the texture
			//int len = lstrlen((LPCWSTR)d3dxMaterials2[i].pTextureFilename);
			//wchar_t *wText = new wchar_t[len];
			//::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterials2[i].pTextureFilename, -1, wText,len );
   //         if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
   //                                             wText, 
   //                                             &g_pMeshTextures2[i] ) ) )
   //         {
   //             // If texture is not in current folder, try parent folder
   //             const TCHAR* strPrefix = TEXT("..\\");
   //             const int lenPrefix = lstrlen( strPrefix );
   //             TCHAR strTexture[MAX_PATH];
   //             lstrcpyn( strTexture, strPrefix, MAX_PATH );
   //             lstrcpyn( strTexture + lenPrefix, wText, MAX_PATH - lenPrefix );
   //             // If texture is not in current folder, try parent folder
   //             if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
   //                                                 strTexture, 
   //                                                 &g_pMeshTextures2[i] ) ) )
   //             {
   //                 MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
   //             }
   //         }
   //     }
   // }

   // // Done with the material buffer
   // pD3DXMtrlBuffer->Release();
    return S_OK;
}

// CHAT: initializes game chat.
void directXClass::IniChat()
{
   SIZE charsz;
   charsz.cx = 10; charsz.cy = 15;
   Text *text = new Text(g_pDevice, false, charsz);
   text->setRect(0, 50, 500, 50);
   text->setColor(250, 250, 250, 255);
   //text->setColor(0, 0, 0, 255);
   //text->setColor(250, 0, 0, 255);
   _chat.SetTextFormat(text);
}

// CHAT: Keydown for the chat
bool directXClass::KeyDownChat(WPARAM wParam, HWND hWnd)
{
   static int c = 0;
   static char str[1];
   if(_chat.IsWritingOn())
   {
      if(wParam == VK_RETURN)
      {
         //_client.SendMsg(_chat.GetCurrentMsgC());
         const char *msg = _chat.GetCurrentMsgC();
         _chat.AddMsgToHistory(msg);

         /**char netmsg[3];
         int len = strlen(msg);
         for(int i = 0; i < len; i++)
         {
            char m[1] = {msg[i]};
            //_msgt.CreateMsg(netmsg, MSG_TXT, &_clientID, m);
            //client.SendMsg(netmsg);
         }
         char m[1] = {NULL};
         _msgt.CreateMsg(netmsg, MSG_TXT, &_clientID, m);
         _client.SendMsg(netmsg);
         **/
         _chat.ClearCurrentMsg();
         //_chat.SendMsg();
         //_server.BroadcastMsg(_chat.GetCurrentMsg().c_str());

         _chat.EndWrite();
      }
      else if(wParam == VK_BACK)
         _chat.DeleteLastChar();
      else
         _chat.Write(wParam);
      
      return true;
   }
   else
   {
      if(wParam == 'Y')   
      {
         _chat.StartWrite();
         return true;
      }
      if(wParam == 'P')
      {
         //CreateServer(hWnd);
         _chat.AddMsgToHistory("Server Created!");
         return true;
      }
      if(wParam == 'L')
      {
         //CreateClient(hWnd, "192.168.0.198");
         _chat.AddMsgToHistory("Client connected to server .198!");
         return true;
      }
      if(wParam == 'O')
      {
         //CreateClient(hWnd, "192.168.0.190");
         ///CreateClient(hWnd, "localhost");
         _chat.AddMsgToHistory("Client connected to server Localhost!");
         return true;
      }
      if(wParam == 'M')
      {
         sprintf(str, "%d", c++);
         ///_server.BroadcastMsg(str, 1);
         if(c >= 10)
            c=0;
         return true;
      }
   }

   return false;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID directXClass::Cleanup()
{
    if( g_pMeshMaterials != NULL ) 
        delete[] g_pMeshMaterials;

    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if( g_pMesh != NULL )
        g_pMesh->Release();
    
    if( g_pDevice != NULL )
        g_pDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

D3DXMATRIX directXClass::Translate(const float dx, const float dy, const float dz) {
    D3DXMATRIX ret;

    D3DXMatrixIdentity(&ret);
    ret(3, 0) = dx;
    ret(3, 1) = dy;
    ret(3, 2) = dz;
    return ret;
}    // End of Translate

void directXClass::inputCommands()
{
	if(input.get_keystate(DIK_A))
	{
		player1.bankLeft(0.05f);      
	}

	if(input.get_keystate(DIK_D))
	{
		player1.bankRight(0.05f);
	}
	
	if(input.get_keystate(DIK_W))
	{
      //if(player1.getPositionZ() > -1.5)
		player1.bankUp(0.05f);
      
	}

	if(input.get_keystate(DIK_S))
	{
      //if(player1.getPositionZ() < 1.5)
		player1.bankDown(0.05f);
	}

	if(input.get_keystate(DIK_SPACE))
	{
		//player1.useAfterBooster();
	}
}

void directXClass::setupCubes()
{
	for(int i = 0; i < 100; ++i)
	{
		D3DXCreateBox(g_pDevice, 0.25f, 0.25f, 0.25f, &cubemesh[i], NULL);
	}
}

void directXClass::cleanupCubes()
{
	for(int i = 0; i < 100; ++i)
	{
		cubemesh[i] = NULL;
	}
}

void directXClass::drawCubes()
{
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, 0.0f, -1.0f, 0.0f);
	g_pDevice->SetTransform( D3DTS_WORLD, &translate);
	for(int i = 0; i < 100; ++i)
	{
		D3DXMatrixTranslation(&translate, 0.0f, 0.0f, ((float) i * 10.0f) / 2.0f);
		g_pDevice->SetTransform( D3DTS_WORLD, &translate);
		cubemesh[i]->DrawSubset(0);
	}
}

point directXClass::getMouseCoordinates() {
	return point(currentX, currentY);
}
