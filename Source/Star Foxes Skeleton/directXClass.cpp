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
float directXClass::lastTime = 0.0f;
int directXClass::currentX = 0;
int directXClass::currentY = 0;
bool directXClass::waiting = true;
Terrain directXClass::mainTerrain = Terrain();

//deals with revaildating the window, and the basic window stuff
long CALLBACK directXClass::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam){
	switch(uMessage){
	case WM_SIZE:
		{
			return 0;
		}
	case WM_CREATE:
		{
			program->hwndDialog = CreateDialog (((LPCREATESTRUCT) lParam)->hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 
                                  g_hWndMain, startDialog) ;
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO1,IDC_RADIO1), MAKELPARAM(IDC_RADIO1,IDC_RADIO1));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO4,IDC_RADIO4), MAKELPARAM(IDC_RADIO4,IDC_RADIO4));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO8,IDC_RADIO8), MAKELPARAM(IDC_RADIO8,IDC_RADIO8));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO10,IDC_RADIO10), MAKELPARAM(IDC_RADIO10,IDC_RADIO10));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO15,IDC_RADIO15), MAKELPARAM(IDC_RADIO15,IDC_RADIO15));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO16,IDC_RADIO16), MAKELPARAM(IDC_RADIO16,IDC_RADIO16));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO21,IDC_RADIO21), MAKELPARAM(IDC_RADIO21,IDC_RADIO21));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO22,IDC_RADIO22), MAKELPARAM(IDC_RADIO22,IDC_RADIO22));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO27,IDC_RADIO27), MAKELPARAM(IDC_RADIO27,IDC_RADIO27));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO28,IDC_RADIO28), MAKELPARAM(IDC_RADIO28,IDC_RADIO28));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO33,IDC_RADIO33), MAKELPARAM(IDC_RADIO33,IDC_RADIO33));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO34,IDC_RADIO34), MAKELPARAM(IDC_RADIO34,IDC_RADIO34));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO39,IDC_RADIO39), MAKELPARAM(IDC_RADIO39,IDC_RADIO39));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO40,IDC_RADIO40), MAKELPARAM(IDC_RADIO40,IDC_RADIO40));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO45,IDC_RADIO45), MAKELPARAM(IDC_RADIO45,IDC_RADIO45));
			SendMessage(program->hwndDialog, WM_COMMAND, MAKELPARAM(IDC_RADIO46,IDC_RADIO46), MAKELPARAM(IDC_RADIO46,IDC_RADIO46));
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
   case WM_SERVER:
      program->OnServerSocketEvent(lParam, wParam, hWnd);
      return 0;      
   case WM_CLIENT:
      program->OnClientSocketEvent(lParam, hWnd);
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
		SetError(TEXT("could not load radar bitmap surface"));
	}

	r=LoadBitmapToSurface(TEXT("radarE.bmp"), &radarE, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load radarE bitmap surface"));
	}

	r=LoadBitmapToSurface(TEXT("radarA.bmp"), &radarA, g_pDevice);
	if(FAILED(r)){
		SetError(TEXT("could not load radarA bitmap surface"));
	}

	lastTime = (float) timeGetTime();

	while(TRUE){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			float currTime = (float) timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f; // get elapsed time since last update in seconds
			if(timeDelta > 0.02f) // Added temp FPS control for multiplayer
         {GameLoop(timeDelta);
			lastTime = currTime;
         }
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
	//OutputDebugString(TEXT("\n"));
	printf("error!!");
}


//initializes the game
int directXClass::GameInit(){
	HRESULT r = 0;//return values
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object

	if(g_pD3D == NULL){
		SetError(TEXT("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
	
	r = InitDirect3DDevice(g_hWndMain, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOWED, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	if(FAILED(r)){//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(TEXT("Initialization of the device failed"));
		return E_FAIL;
	}
	
	InitGeometry();
	LoadAlphabet(TEXT("Alphabet vSmall.bmp"), 8, 16);
	InitTiming();
	std::wstring name = (WCHAR*)"Human";
	/*dummyAI = LightShipClass(g_pMeshLight, g_pMeshMaterialsLight, g_pMeshTexturesLight, g_dwNumMaterialsLight, g_pDevice);
	player1 = HumanPlayerClass(dummyAI, name, 0, 1);
	player1.initProjectiles(g_pMeshLaser, g_pMeshMaterialsLaser, g_pMeshTexturesLaser, g_dwNumMaterialsLaser);*/

	mainTerrain = Terrain(g_pMesh2, g_pMeshMaterials2, g_pMeshTextures2, g_dwNumMaterials2, g_pDevice);
	mainTerrain.SetMtrlColor(D3DXCOLOR(0.0f, 0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.1f, 0.0f, 0.0f), D3DXCOLOR(0, 0.1f, 0.0f, 0.0f));
	mainTerrain.setupBuildings();

	

	static D3DMATERIAL9* mat1 = new D3DMATERIAL9;
   
	if(FAILED(r)){//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(TEXT("Initialization of the device failed"));
		return E_FAIL;
	}

	input.init_keyboard(g_hWndMain);
	setupCubes();
	/*MainPlayerClass *allPlayers[2];
	allPlayers[0] = &player1;
	allPlayers[1] = &ai1;*/
	//_gamestate.updateGameState(g_hWndMain, allPlayers, 2);

	return S_OK;
}

//the game loop, renders, counts frames, and quits on esc key down
int directXClass::GameLoop(float timeDelta) {
	int nullCount = 0;
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
				ShowWindow(hwndDialog, SW_SHOW) ;
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
      {/* WORD vkCode = 0x36; // '6'
      INPUT keyEvent = {0};
      keyEvent.type = INPUT_KEYBOARD;
      keyEvent.ki.wVk = vkCode;
      keyEvent.ki.wScan = MapVirtualKeyEx(vkCode, 0, (HKL)0xf0010413);
      SendInput(1, &keyEvent, sizeof(keyEvent));*/
        /* if(_IamServer)
         {
            //Sleep(5);
            _msgt.CreateMsg(_netmsg, MSG_MSC, MSC_INIFRAME, "0");
            _server.BroadcastMsg(_netmsg, 3);
         }
         if(_IamClient) 
            if(!_iniframe) { 
            WORD vkCode = 0x36; // '6'
            INPUT keyEvent = {0};
            keyEvent.type = INPUT_KEYBOARD;
            keyEvent.ki.wVk = vkCode;
            keyEvent.ki.wScan = MapVirtualKeyEx(vkCode, 0, (HKL)0xf0010413);
            SendInput(1, &keyEvent, sizeof(keyEvent));
               return 0;
            }; //sync start of frame*/
         
         if(!_IamServer && !_IamClient)
         {
            StartAIs();
         }

			inputCommands(timeDelta);

         for (int i = 0; i < 8; i++)
			{
				if (program->currentPlayers[i] != NULL) {
					program->currentPlayers[i]->Update(timeDelta);
				}
			}
			//player1.Update(timeDelta);

			/*if(input.get_keystate(DIK_M))
			{
				player1.takeHit(5);
			}
			if(input.get_keystate(DIK_N))
			{
				///player1.takeHit(-5);
				player1.startBlinking();
			}*/

			/*if(input.get_keystate(DIK_M))
			{
				currentPlayers[1]->takeHit(5);
			}*/
			/*if(input.get_keystate(DIK_N))
			{
				currentPlayers[1]->takeHit(5);
			}*/

			
			if(input.get_keystate(DIK_Q))
			{
				menuSelect = 0;
				return 0;
			}

			for (int index = 0; index < 8; index++) {
				if (currentPlayers[index] != NULL) {
					if (currentPlayers[index]->getShipCurrentHealth() < 0 ) {
						currentPlayers[index]->reduceLives();
						RespawnPlayer(currentPlayers[index]);
						if (currentPlayers[index]->getLives() <= 0) {
							currentPlayers[index] = NULL;
							if (index == 0) {
								menuSelect = 0;
								return 0;
							}
						}
					}
				} if (currentPlayers[index] == NULL || currentPlayers[index]->getTeamNum() == currentPlayers[0]->getTeamNum()) {
					nullCount++;
				}
			}
			if (nullCount >= 8) {
				menuSelect = 0;
			}		

			updateCameraTarget();
			camera.Update(timeDelta);

			

			//Check to see if any of the players/projectiles are colliding with buildings
			for (int playerIndex = 0; playerIndex < 8; playerIndex++)
			{
				if (currentPlayers[playerIndex] != NULL) 
				{
					for(int buildingIndex = 0; buildingIndex < 19; buildingIndex++)
					{
						if(boundingBox::checkCollision(currentPlayers[playerIndex]->GetShip()->getPositionVector(), mainTerrain.buildingBounds[buildingIndex]))
						{
							currentPlayers[playerIndex]->takeHit(1);
						}

						std::list<Projectile>::iterator it = currentPlayers[playerIndex]->GetShip()->projectileList.begin();

						while(it != currentPlayers[playerIndex]->GetShip()->projectileList.end())
						{
							for(int buildingIndex = 0; buildingIndex < 19; buildingIndex++)
							{
								if(boundingBox::checkCollision((it->getPosition() + currentPlayers[playerIndex]->GetShip()->getPositionVector()), mainTerrain.buildingBounds[buildingIndex]))
								{
									it->setTimeToLive(0);
								}
							}
							++it;
						}
					}
				} 
			}

			//Check to see if any of the projectiles are colliding with players
			for (int playerIndex = 0; playerIndex < 8; playerIndex++)
			{
				if (currentPlayers[playerIndex] != NULL) 
				{	
					std::list<Projectile>::iterator it = currentPlayers[playerIndex]->GetShip()->projectileList.begin();

					while(it != currentPlayers[playerIndex]->GetShip()->projectileList.end())
					{
						for (int shotPlayerIndex = 0; shotPlayerIndex < 8; shotPlayerIndex++)
						{
							if (currentPlayers[shotPlayerIndex] != NULL && shotPlayerIndex != playerIndex) 
							{
								if(boundingBox::checkCollision((it->getPosition() + currentPlayers[playerIndex]->GetShip()->getPositionVector()), currentPlayers[shotPlayerIndex]->GetShip()->getBound()))
								{
									currentPlayers[shotPlayerIndex]->takeHit(currentPlayers[shotPlayerIndex]->GetShip()->getDamagePerShot());
									it->setTimeToLive(0);
								}
							}
						}
						++it;
					}
				} 
			}

			updateCameraTarget();
			camera.Update(timeDelta);//reset();
			UpdateHUD();



			//directXClass::SetError(TEXT("p1: %f"), player1.getPositionZ());
/*			for (std::list<AIPlayer*>::const_iterator ci = _aiPlayer.begin(); ci != _aiPlayer.end(); ++ci)
			{
				//directXClass::SetError(TEXT("p1: %f"), player1.getRotation().z); 
				//(*ci)->bankLeft(0.01f);
				//MessageBoxA(g_hWndMain, "hi", "hilo", 0);
				(*ci)->Update(timeDelta);
            //(*ci)->updatePosition(timeDelta);
				//(*ci)->bankUp(0.04f);
				// (*ci)->updateRotation();
				//(*ci)->updatePosition();
            
			}*/
			//_aiPlayer.front()->bankLeft(0.01f);
			//_aiplayer1.bankLeft(0.01f);
			//player2.(0.01f);
            Render();
            }
		break;

		// In Multiplayer game, currently the same as single player as there
		// is currently no difference
		case 3:
			inputCommands(timeDelta);

			//player1.updatePosition(timeDelta);

			updateCameraTarget();
			camera.reset();//Update(timeDelta);

			if(input.get_keystate(DIK_M))
			{
				player1.takeHit(5);
			}
			if(input.get_keystate(DIK_N))
			{
				player1.takeHit(-5);
			}
			UpdateHUD();
			Render();
		break;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

//Networking: Starts AI altogether.
void directXClass::StartAIs()
{
	for (int i = 1; i < 8; i++)
	{
		if (program->currentPlayers[i] != NULL) {
			program->currentPlayers[i]->Start();
		}
	}
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
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,100,100), 1.0f, 0);

    // Clear the zbuffer
    g_pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
 
	D3DLOCKED_RECT Locked;

	//get pointer to backbuffer
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer"));
	}
	
	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface"));
	pBackSurf->Release();//release lock
	
	pBackSurf = 0;

	// Begin the scene
	if( SUCCEEDED( g_pDevice->BeginScene() ) )
	{
		setupLights();

		g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

		// Setup the world, view, and projection matrices
		SetupMatrices(true);

		D3DXMATRIX mView = camera.getViewMatrix();
		g_pDevice->SetTransform(D3DTS_VIEW, &mView);
       ///
      LPD3DXLINE line;
        float size = 0.1f;
        //D3DXVECTOR2 loc(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        D3DXVECTOR3 loc(0.0f, 0.0f, 0.0f);
        D3DXCreateLine(g_pDevice, &line);
        D3DXVECTOR3 lines[] = 
        {D3DXVECTOR3(loc.x, loc.y, loc.z), D3DXVECTOR3(loc.x + size, loc.y, loc.z),
        D3DXVECTOR3(loc.x + size, loc.y, loc.z), D3DXVECTOR3(loc.x + size, loc.y + size, loc.z),
        D3DXVECTOR3(loc.x + size, loc.y + size, loc.z), D3DXVECTOR3(loc.x, loc.y + size, loc.z),
        D3DXVECTOR3(loc.x, loc.y + size, loc.z), D3DXVECTOR3(loc.x, loc.y, loc.z),
        };
        line->Begin();
        //line->sett
        D3DXMATRIX translate;
        D3DXMatrixTranslation(&translate, -0.05f, 0.8f, 0.96f);
        //g_pDevice->SetTransform( D3DTS_WORLD, &translate);   
        line->SetWidth(5.5f);
        line->DrawTransform(lines, 8, &translate, 0xFFFFFFFF);
        line->End();
        line->Release();
        ////
		mainTerrain.renderSelf();	
		player1.drawSelf(mView, false);
		for (int i = 1; i < 8; i++) {
			if (currentPlayers[i] != NULL) {
				currentPlayers[i]->drawSelf(mView, true);
			}
		}

		
		//player2.drawSelf();
		//SetupMatrices(true);
     
		//drawCubes();
		
		

      _chat.RenderChat();
      /*
      for (std::list<AIPlayer*>::const_iterator ci = _aiPlayer.begin(); ci != _aiPlayer.end(); ++ci)
      {
         (*ci)->drawSelf();
      }	*/
		
		// End the scene
        
        
		g_pDevice->EndScene();
	}
	
	//get pointer to backbuffer for HUD
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer"));
	}
	pBackSurf->LockRect(&Locked, 0, 0);
	PrintFrameRate(0, 0, TRUE, D3DCOLOR_ARGB(255,255,0,255), (DWORD*)Locked.pBits, Locked.Pitch);
	pBackSurf->UnlockRect();

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &healthRect, healthSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface health"));

	r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &radarRect, radarSurface, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
	if(FAILED(r))
		SetError(TEXT("did not copy surface health"));

	pBackSurf->Release();//release lock
	
	pBackSurf = 0;
	RenderRadar();
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

int directXClass::RenderRadar()
{
	HRESULT r;
	LPDIRECT3DSURFACE9 pBackSurf = 0;
	r=g_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if(FAILED(r)){
		SetError(TEXT("Couldn't get backbuffer in render radar"));
	}

	D3DXVECTOR3 positionVectors[8];
	D3DXVECTOR3 player1Pos = currentPlayers[0]->getPositionVector();

	for (int index = 0; index < 8; index++) {
		if (currentPlayers[index] != NULL) {
			positionVectors[index] = currentPlayers[index]->getPositionVector();
			positionVectors[index] -= player1Pos;

			D3DXMATRIX rotateRadar = D3DXMATRIX(1,0,0,0
											   ,0,1,0,0
											   ,0,0,1,0
											   ,0,0,0,1);

			float angleToRotate;
			float hypot = std::sqrt((currentPlayers[0]->GetShip()->getDirectionVector().z * currentPlayers[0]->GetShip()->getDirectionVector().z)
								 +(currentPlayers[0]->GetShip()->getDirectionVector().x * currentPlayers[0]->GetShip()->getDirectionVector().x));			
			
			if(currentPlayers[0]->GetShip()->getDirectionVector().x >= 0 &&
				currentPlayers[0]->GetShip()->getDirectionVector().z >= 0)
			{
				angleToRotate = std::atan(currentPlayers[0]->GetShip()->getDirectionVector().z /
											currentPlayers[0]->GetShip()->getDirectionVector().x);
			}
			else if(currentPlayers[0]->GetShip()->getDirectionVector().x >= 0 &&
				currentPlayers[0]->GetShip()->getDirectionVector().z <= 0)
			{
				angleToRotate = std::asin(currentPlayers[0]->GetShip()->getDirectionVector().z / hypot);
			}
			else if(currentPlayers[0]->GetShip()->getDirectionVector().x <= 0 &&
				currentPlayers[0]->GetShip()->getDirectionVector().z >= 0)
			{
				angleToRotate = std::acos(currentPlayers[0]->GetShip()->getDirectionVector().x / hypot);
			}
			else if(currentPlayers[0]->GetShip()->getDirectionVector().x <= 0 &&
				currentPlayers[0]->GetShip()->getDirectionVector().z <= 0)
			{
				angleToRotate = std::atan(currentPlayers[0]->GetShip()->getDirectionVector().z /
											currentPlayers[0]->GetShip()->getDirectionVector().x);
				angleToRotate -= D3DX_PI;
			}

			D3DXMatrixRotationY( &rotateRadar, (angleToRotate - (D3DX_PI / 2)));
			D3DXVec3TransformCoord(&positionVectors[index], &positionVectors[index], &rotateRadar);
			
			positionVectors[index].z *= -1;

			positionVectors[index] += D3DXVECTOR3(540,0,400);

			if(positionVectors[index].x > 591)
			{
				positionVectors[index].x = 591;
			}
			if(positionVectors[index].x < 489)
			{
				positionVectors[index].x = 489;
			}
			if(positionVectors[index].z < 349)
			{
				positionVectors[index].z = 349;
			}
			if(positionVectors[index].z > 451)
			{
				positionVectors[index].z = 451;
			}
				
			radarPlayers[index].bottom = positionVectors[index].z + 5;
			radarPlayers[index].top = positionVectors[index].z - 5;
			radarPlayers[index].left = positionVectors[index].x - 5;
			radarPlayers[index].right = positionVectors[index].x + 5;

			if(currentPlayers[index]->getTeamNum() == currentPlayers[0]->getTeamNum())
			{
				r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &radarPlayers[index], radarA, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
				if(FAILED(r))
					SetError(TEXT("did not copy surface radarA"));
			}
			else
			{
				r=D3DXLoadSurfaceFromSurface(pBackSurf, NULL, &radarPlayers[index], radarE, NULL, NULL, D3DX_FILTER_TRIANGLE,0);
				if(FAILED(r))
					SetError(TEXT("did not copy surface radarA"));
			}
			
		} else {
			positionVectors[index] = D3DXVECTOR3(0,0,0);
		}
	}

	

	pBackSurf->Release();//release lock
	
	pBackSurf = 0;
	return S_OK;
}

int directXClass::UpdateHUD()
{
	healthRect.right = (LONG) (400 * (player1.getShipCurrentHealth() / (float)player1.getShipMaxHealth()) + 60);
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

	
    D3DXVECTOR3 vEyePt( 0.0f, 1.0f, -15.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	
    D3DXMATRIXA16 matView2;
	 D3DXMatrixLookAtLH(&matView2, &vEyePt, &vLookatPt, &vUpVec);

    g_pDevice->SetTransform( D3DTS_VIEW, &matView2 );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 220.0f );
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

    if( FAILED( D3DXLoadMeshFromX( TEXT("terrain.x"), D3DXMESH_SYSTEMMEM, 
                                   g_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials2, 
                                   &g_pMesh2 ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( TEXT("..\\terrain.x"), D3DXMESH_SYSTEMMEM, 
                                    g_pDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials2, 
                                    &g_pMesh2 ) ) )
        {
            MessageBox(NULL, TEXT("Could not find terrain.x"), TEXT("Meshes.exe"), MB_OK);
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials2 = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials2 = new D3DMATERIAL9[g_dwNumMaterials2];
    g_pMeshTextures2  = new LPDIRECT3DTEXTURE9[g_dwNumMaterials2];

    for( DWORD i=0; i<g_dwNumMaterials2; i++ )
    {
        // Copy the material
        g_pMeshMaterials2[i] = d3dxMaterials2[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterials2[i].Ambient = g_pMeshMaterials2[i].Diffuse;

        g_pMeshTextures2[i] = NULL;
        if( d3dxMaterials2[i].pTextureFilename != NULL && 
            lstrlen((LPCWSTR)d3dxMaterials2[i].pTextureFilename) > 0 )
        {
            // Create the texture
			int len = lstrlen((LPCWSTR)d3dxMaterials2[i].pTextureFilename);
			wchar_t *wText = new wchar_t[len];
			::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterials2[i].pTextureFilename, -1, wText,len );
            if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                wText, 
                                                &g_pMeshTextures2[i] ) ) )
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
                                                    &g_pMeshTextures2[i] ) ) )
                {
                    MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
                }
            }
        }
    }

	if( FAILED( D3DXLoadMeshFromX( TEXT("big_airwing.x"), D3DXMESH_SYSTEMMEM, 
                                   g_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsHeavy, 
                                   &g_pMeshHeavy ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( TEXT("..\\big_airwing.x"), D3DXMESH_SYSTEMMEM, 
                                    g_pDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsHeavy, 
                                    &g_pMeshHeavy ) ) )
        {
            MessageBox(NULL, TEXT("Could not find big_airwing.x"), TEXT("Meshes.exe"), MB_OK);
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterialsHeavy = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterialsHeavy = new D3DMATERIAL9[g_dwNumMaterialsHeavy];
    g_pMeshTexturesHeavy  = new LPDIRECT3DTEXTURE9[g_dwNumMaterialsHeavy];

    for( DWORD i=0; i<g_dwNumMaterialsHeavy; i++ )
    {
        // Copy the material
        g_pMeshMaterialsHeavy[i] = d3dxMaterialsHeavy[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterialsHeavy[i].Ambient = g_pMeshMaterialsHeavy[i].Diffuse;

        g_pMeshTexturesHeavy[i] = NULL;
        if( d3dxMaterialsHeavy[i].pTextureFilename != NULL && 
            lstrlen((LPCWSTR)d3dxMaterialsHeavy[i].pTextureFilename) > 0 )
        {
            // Create the texture
			int len = lstrlen((LPCWSTR)d3dxMaterialsHeavy[i].pTextureFilename);
			wchar_t *wText = new wchar_t[len];
			::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterialsHeavy[i].pTextureFilename, -1, wText,len );
            if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                wText, 
                                                &g_pMeshTexturesHeavy[i] ) ) )
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
                                                    &g_pMeshTexturesHeavy[i] ) ) )
                {
                    MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
                }
            }
        }
    }

   // Done with the material buffer
    pD3DXMtrlBuffer->Release();

	if( FAILED( D3DXLoadMeshFromX( TEXT("small_airwing.x"), D3DXMESH_SYSTEMMEM, 
                                   g_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsLight, 
                                   &g_pMeshLight ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( TEXT("..\\small_airwing.x"), D3DXMESH_SYSTEMMEM, 
                                    g_pDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsLight, 
                                    &g_pMeshLight ) ) )
        {
            MessageBox(NULL, TEXT("Could not find small_airwing.x"), TEXT("Meshes.exe"), MB_OK);
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterialsLight = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterialsLight = new D3DMATERIAL9[g_dwNumMaterialsLight];
    g_pMeshTexturesLight  = new LPDIRECT3DTEXTURE9[g_dwNumMaterialsLight];

    for( DWORD i=0; i<g_dwNumMaterialsLight; i++ )
    {
        // Copy the material
        g_pMeshMaterialsLight[i] = d3dxMaterialsHeavy[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterialsLight[i].Ambient = g_pMeshMaterialsLight[i].Diffuse;

        g_pMeshTexturesLight[i] = NULL;
        if( d3dxMaterialsLight[i].pTextureFilename != NULL && 
            lstrlen((LPCWSTR)d3dxMaterialsLight[i].pTextureFilename) > 0 )
        {
            // Create the texture
			int len = lstrlen((LPCWSTR)d3dxMaterialsLight[i].pTextureFilename);
			wchar_t *wText = new wchar_t[len];
			::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterialsLight[i].pTextureFilename, -1, wText,len );
            if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                wText, 
                                                &g_pMeshTexturesLight[i] ) ) )
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
                                                    &g_pMeshTexturesLight[i] ) ) )
                {
                    MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
                }
            }
        }
    }

   // Done with the material buffer
    pD3DXMtrlBuffer->Release();

	if( FAILED( D3DXLoadMeshFromX( TEXT("laser.x"), D3DXMESH_SYSTEMMEM, 
                                   g_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsLaser, 
                                   &g_pMeshLaser ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( TEXT("..\\Laser.x"), D3DXMESH_SYSTEMMEM, 
                                    g_pDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &g_dwNumMaterialsLaser, 
                                    &g_pMeshLaser ) ) )
        {
            MessageBox(NULL, TEXT("Could not find Laser.x"), TEXT("Meshes.exe"), MB_OK);
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterialsLaser = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterialsLaser = new D3DMATERIAL9[g_dwNumMaterialsLaser];
    g_pMeshTexturesLaser  = new LPDIRECT3DTEXTURE9[g_dwNumMaterialsLaser];

    for( DWORD i=0; i<g_dwNumMaterialsLight; i++ )
    {
        // Copy the material
        g_pMeshMaterialsLaser[i] = d3dxMaterialsHeavy[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterialsLaser[i].Ambient = g_pMeshMaterialsLaser[i].Diffuse;

        g_pMeshTexturesLaser[i] = NULL;
        if( d3dxMaterialsLaser[i].pTextureFilename != NULL && 
            lstrlen((LPCWSTR)d3dxMaterialsLaser[i].pTextureFilename) > 0 )
        {
            // Create the texture
			int len = lstrlen((LPCWSTR)d3dxMaterialsLaser[i].pTextureFilename);
			wchar_t *wText = new wchar_t[len];
			::MultiByteToWideChar(  CP_ACP, NULL,d3dxMaterialsLaser[i].pTextureFilename, -1, wText,len );
            if( FAILED( D3DXCreateTextureFromFile( g_pDevice, 
                                                wText, 
                                                &g_pMeshTexturesLaser[i] ) ) )
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
                                                    &g_pMeshTexturesLaser[i] ) ) )
                {
                    MessageBox(NULL, TEXT("Could not find texture map"), TEXT("Meshes.exe"), MB_OK);
                }
            }
        }
    }

   // Done with the material buffer
    pD3DXMtrlBuffer->Release();
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
         //_chat.AddMsgToHistory(msg);

         char netmsg[3];
         int len = strlen(msg);
         //sending txt msg char by char.
         for(int i = 0; i < len; i++)
         {
            char m[1] = {msg[i]};
            _msgt.CreateMsg(netmsg, MSG_TXT, &_clientID, m);
            _client.SendMsg(netmsg);
         }
         //sending null to terminate txt msg.
         char m[1] = {NULL};
         _msgt.CreateMsg(netmsg, MSG_TXT, &_clientID, m);
         _client.SendMsg(netmsg);
         
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
         CreateServer(hWnd);
         _chat.AddMsgToHistory("Server Created!");
         return true;
      }
      if(wParam == 'L')
      {
         CreateClient(hWnd, "localhost");
         //CreateClient(hWnd, "192.168.0.198");
         _chat.AddMsgToHistory("Client connected to server .198!");
         return true;
      }
      if(wParam == 'O')
      {
         //CreateClient(hWnd, "192.168.0.190");
         CreateClient(hWnd, "localhost");
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

//Networking: processing server socket message
void directXClass::OnServerSocketEvent(LPARAM lParam, WPARAM wParam, HWND hWnd)
{
   //static int ii = 0;
   static bool setID;
   int* msgsize = new int;
   //Game* game = (Game*)GetWindowLong(hWnd, 0);
   //Server* server = game->GetServer();
   //MsgTranslator* msgt = _msgt;
   
   switch(_server.OnSocketEvent(lParam, wParam, msgsize, hWnd))
   {
   case EV_ACCEPT:
      {
         _chat.AddMsgToHistory("Client connected!");       
         char msg[10];
         char id[10];
         itoa(_server.GenID(), id, 10);
         int size = _msgt.CreateMsg(msg, MSG_MSC, MSC_SETID, id);
         //server->SendMsg(server->GetBackList(), msg, strlen(msg));
         _server.SendMsg(_server.GetBacklist(), msg, size);
         break;
      }
   case EV_CLOSE:
      _chat.AddMsgToHistory("Client closed connection!");
      break;
   case EV_READ:
      /*char msg[5];
      
      sprintf(msg, "%d", ii++);
      int len = strlen(msg);
      msg[len] = NULL;*/
      int len;
      const char* msg = (char*) malloc(128);
      msg = _server.PopFrontMsg(&len);
      //game->GetChat()->AddMsgToHistory(msg);
      
      _chat.AddMsgToHistory(msg);
      _server.BroadcastMsg(msg, len);
      break;
   }
}

// Networking: processing client socket message.
void directXClass::OnClientSocketEvent(LPARAM lParam, HWND hWnd)
{
   //Game* game = (Game*)GetWindowLong(hWnd, 0);
   //Client* client = game->GetClient();
   switch(_client.OnSocketEvent(lParam, hWnd))
   {
   case EV_READ:
      {
      //string str = client->PopRcvdMsg();
      //MsgTranslator* msgt = game->GetMsgTranslator();      
      _msgt.SetMsg(_client.PopRcvdMsg());

      //msgt->TranslateMsg(client->PopRcvdMsg());
      //const char* msg = msgt->GetMsg()->GetMsg();
      
      //game->GetChat()->AddMsgToHistory(msg);
      ProcessMsg(_msgt.GetMsg(), hWnd);
      //if(msg[3] == 'l')
      //game->GetMeshes()->GetSelectedWorld()->KeyDown(VK_LEFT);
      break;
      }
   case EV_SERVER_ERR:
      _chat.AddMsgToHistory("Connection to server failed");
      break;
   case EV_CLOSE:
			
      break;
   }
}

//Networking: processes network msgs
void directXClass::ProcessMsg(Msg* msg, HWND hWnd)
{
   switch(msg->GetType())
   {
   case MSG_CMD:
      //_chat.AddMsgToHistory("processing type cmd");
      ProcessClientCmd(msg, hWnd);
      break;   
   case MSG_TXT:
      ProcessTxt(msg);
      break;
   case MSG_MSC:
      ProcessMsc(msg);
      break;
   }
}

// Processes miscelaneus messages.
void directXClass::ProcessMsc(Msg *msg)
{
   switch(msg->GetCmd())
   {
   case MSC_SETID:
     { _clientID = msg->GetBody();
      std::string str = "My ClientID is: ";
      char n[2] = {_clientID, NULL};
      str.append(n);
      _chat.AddMsgToHistory(str);
     }
      break;
   case MSC_INIFRAME:
      _iniframe = true;
      break;
   case MSC_STARTGAME:
      this->StartAIs();
   default:
      //std::string str; 
      char a[2] = {msg->GetBody(), NULL};
      //str.append(a);
      _chat.AddMsgToHistory(a);
   }
}

void directXClass::ProcessTxt(Msg* msg)
{
   static char m1[128];
   static char m2[128];   
   static int c1 = 0;
   static int c2 = 0;

   char* m;
   int* c;

   if(msg->GetCmd() == '1')
   {
      m = m1;
      c = &c1;
   }
   else
   {
      m = m2;
      c = &c2;
   }

   if(*c == 0)
   {
      m1[(*c)++] = msg->GetCmd();
      m1[(*c)++] = ':';
      m1[(*c)++] = ' ';
   }

   if(*c >= 127)
      return;
   
   m1[(*c)++] = msg->GetBody();
   if(msg->GetBody() == NULL)
   {
      _chat.AddMsgToHistory(m1);
      *c = 0;
   }
}

void directXClass::ProcessClientCmd(Msg* msg, HWND hWnd)
{
   //_chat.AddMsgToHistory("processing client key");
/*   switch(msg->GetCmd())
   {
   case MV_LEFT:
      //_clientID = msg->GetBody()[0];
      GetMeshes()->GetSelectedWorld()->KeyDown(VK_LEFT);
      break;
   case MV_UP:
      GetMeshes()->GetSelectedWorld()->KeyDown(VK_UP);
      break;
   case MV_DOWN:
      GetMeshes()->GetSelectedWorld()->KeyDown(VK_DOWN);
      break;
   case MV_RITE:
      GetMeshes()->GetSelectedWorld()->KeyDown(VK_RIGHT);
      break;
   case '':

   }
   */
   const char x[1] = {msg->GetCmd()};
   int num = atoi(x);

   switch(msg->GetBody())
   {
   case 'A': { // go left
      currentPlayers[num]->right(false);//player1.right(false);
      currentPlayers[num]->left(true);//player1.left(true);
      
      //player1.Update(0.03f);
      /*std::string a; char e[2] = {msg->GetCmd(), NULL};
      a.append("A: ");
      a.append(e);
      _chat.AddMsgToHistory(a);*/
             }
             break;
   case 'D': //go right
      currentPlayers[num]->left(false);//player1.left(false);
      currentPlayers[num]->right(true);//player1.right(true);      
      //player1.Update(0.03f);
      //_chat.AddMsgToHistory("D PRESSED");
      break;
   case 'X': //stops horizontal rotation
      currentPlayers[num]->right(false);//player1.right(false);
      currentPlayers[num]->left(false);//player1.left(false);
      //player1.Update(_timeDelta);
      //_chat.AddMsgToHistory("X PRESSED");
      break;
   case 'W': // go down
      currentPlayers[num]->up(false);//player1.up(false);
      currentPlayers[num]->down(true);//player1.down(true);
      //player1.Update(0.03f);
      break;
   case 'S': // go up
      currentPlayers[num]->down(false);//player1.down(false);
      currentPlayers[num]->up(true);//player1.up(true);
      //player1.Update(0.03f);
      break;
   case 'Z': //stops vertical rotation
      currentPlayers[num]->up(false);//player1.up(false);
      currentPlayers[num]->down(false);//player1.down(false);
      //player1.Update(0.03f);
      break;
   case 'C': //stops all rotations
      currentPlayers[num]->up(false);//player1.up(false);
      currentPlayers[num]->down(false);//player1.down(false);
      currentPlayers[num]->left(false);//player1.up(false);
      currentPlayers[num]->right(false);//player1.down(false);      
      //player1.Update(0.03f);
      break;      
   case '!': // boost on
      currentPlayers[num]->boost(true);//player1.boost(true);
      break;
   case '@': // boost off
      currentPlayers[num]->boost(false);//player1.boost(false);
      break;
   case 'F': // boost off
      currentPlayers[num]->shoot(_timeDelta);//player1.shoot(_timeDelta);
      break;
   }
   //player1.Update(_timeDelta);
   /*if(num >= 2)
   {
      GetMeshes()->GetWorldByIndex(num)->KeyDown(msg->GetCmd());
      return;
   }

   if(SwitchMeshCtrl(msg->GetCmd(), this))
   {
     // MessageBoxA(hWnd, "rec", "nothing", 0); 
      return;
   }
   if(GetMeshes()->GetSelectedWorld()->KeyDown(msg->GetCmd()))*/
      return;
}


//Networking: creating server
bool directXClass::CreateServer(HWND hWnd)
{
   //Game* game = (Game*)GetWindowLong(hWnd, 0);
   //directXClass *game = directXClass::program;
   std::string* msg = new std::string[1];
   ESocketError err;
   Server* server = GetServer();
   
   if(!(err = server->IniServer(msg)) == ERR_NONE)
   {
   	MessageBoxA(hWnd,
         msg->c_str(),
			"Error!",
			MB_ICONINFORMATION|MB_OK);
      return false;
   }

   if(!(err = server->AsyncSelect(hWnd, WM_SERVER, msg)) == ERR_NONE)
   {
   	MessageBoxA(hWnd,
         msg->c_str(),
			"Error!",
			MB_ICONINFORMATION|MB_OK);
      return false;
   }
   _IamServer = true;
   return true;
}

//Networking: Create Client
bool directXClass::CreateClient(HWND hWnd, char *hostip)
{
   std::string msg;
   //Game* game = (Game*)GetWindowLong(hWnd, 0);
   //Client* client = GetClient();
   _client.SetHost(hostip);
   if(_client.IniClientSocket(&msg) != ERR_NONE ||
      _client.AsyncSelect(hWnd, WM_CLIENT, &msg) != ERR_NONE ||
      _client.ConnectToHost(&msg) != ERR_NONE
      )
   {
      _chat.AddMsgToHistory(msg.c_str());
      /*MessageBox(hWnd,
         msg.c_str(),
		   "Critical Error",
		   MB_ICONERROR);
	   SendMessage(hWnd,WM_DESTROY,NULL,NULL);*/
      return false;
   }
   _IamClient = true;
   return true;
}

//Networking: Gettting Server
Server* directXClass::GetServer()
{
   return &_server;
}

Client* directXClass::GetClient()
{
   return &_client;
}

// End networking stuff

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

void directXClass::inputCommands(float timeDelta)
{
   static bool offsenth = false; // Networking: not send too many horizont off msgs.
   static bool offsentv = false; // Networking: not send too many vertical off msgs.
   static bool offsentb = false; // Networking: not send too many boost off msgs.
   static int count = 48;

   /*if(input.get_keystate(DIK_Z))
   {
      char c[1] = {count++};
      if(count % 58 == 0)
         count = 48;
      _msgt.CreateMsg(_netmsg, MSG_MSC, "Z", c);
      _client.SendMsg(_netmsg);
      return;
   }*/
   //TODO: REMOVE
   _clientID = '0';
	if(input.get_keystate(DIK_A))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "A");
         _client.SendMsg(_netmsg);
         offsenth = false;
      } else {
		   player1.right(false);
		   player1.left(true);
      }
	}
	else if(input.get_keystate(DIK_D))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "D");
         _client.SendMsg(_netmsg);
         offsenth = false;
      } else {
		   player1.left(false);
		   player1.right(true);
      }
	}
	else
	{
      if(_IamClient){
         if(!offsenth)
         {
            _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "X");
            _client.SendMsg(_netmsg);
         }
         offsenth = true;
      } else {
		   player1.right(false);
		   player1.left(false);
      }
	}

	
	if(input.get_keystate(DIK_W))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "W");
         _client.SendMsg(_netmsg);
         offsentv = false;
      } else {
		   player1.up(false);
		   player1.down(true);
      }
	}
	else if(input.get_keystate(DIK_S))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "S");
         _client.SendMsg(_netmsg);
         offsentv = false;
      } else {
		   player1.down(false);
		   player1.up(true);
      }
	}
	else
	{
      if(_IamClient){
         if(!offsentv)
         {
            _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "Z");
            _client.SendMsg(_netmsg);
         }
         offsentv = true;
      } else {
		   player1.up(false);
		   player1.down(false);
      }
	}

   // If you are server, this starts all AIs at the same time   
   if(input.get_keystate(DIK_T) && _IamServer)
	{
      _msgt.CreateMsg(_netmsg, MSG_MSC, MSC_STARTGAME, "T"); 
      _server.BroadcastMsg(_netmsg, 3);
	}

	if(input.get_keystate(DIK_M) && !player1.gethasShot())
	{
		/*player1.sethasShot(true);*/
	}

	if(!input.get_keystate(DIK_M))
	{
		/*player1.sethasShot(false);*/
	}

	if(input.get_keystate(DIK_L))
	{
		if(dirLightEnabled)
			dirLightEnabled = false;
		else
			dirLightEnabled = true;
	}

	if(input.get_keystate(DIK_SPACE))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "!");
         _client.SendMsg(_netmsg);
         offsentb = false;
      } else {
		   player1.boost(true);
      }
	}
	else
	{
      if(_IamClient){
         if(!offsentb)
         {
            _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "@");
            _client.SendMsg(_netmsg);
         }
         offsentb = true;
      } else {
		   player1.boost(false);
      }
	}

	if(input.get_keystate(DIK_F))
	{
      if(_IamClient){
         _msgt.CreateMsg(_netmsg, MSG_CMD, &_clientID, "F");
         _client.SendMsg(_netmsg);
      } else {
		   player1.shoot(timeDelta);
      }
	}
}

void directXClass::setupCubes()
{
   static float x = 2.0f;
   float w = 0.05f;
   D3DXCreateBox(g_pDevice, w, 10, w, &cubemesh[0], NULL);
   D3DXCreateBox(g_pDevice, w, 10, w, &cubemesh[1], NULL);
   D3DXCreateBox(g_pDevice, 10, w, w, &cubemesh[2], NULL);
   D3DXCreateBox(g_pDevice, 10, w, w, &cubemesh[3], NULL);
   D3DXCreateBox(g_pDevice, w, w, w, &cubemesh[4], NULL);
   //D3DXCreateLine(g_pDevice, w, 12, w, &cubemesh[4], NULL);

	/*for(int i = 5; i < 100; ++i)
	{
		D3DXCreateBox(g_pDevice, x+1.0f, x +0.1f, 0.25f, &cubemesh[i], NULL);
	}*/
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
	D3DXMATRIX translate, translate2;
	D3DXMatrixTranslation(&translate, 0.0f, -1.0f, 0.0f);
	g_pDevice->SetTransform( D3DTS_WORLD, &translate);
   D3DMATERIAL9* mat = new D3DMATERIAL9();
   mat->Ambient= D3DXCOLOR(255.0f, 0, 255.0f, 255.0f);
   g_pDevice->SetMaterial( mat);
	      
   for(int i = 0; i < 5; ++i)
	{
      if(i % 2 != 0)
		   D3DXMatrixTranslation(&translate, -4, 0.0f, ((float) (i+1) * 20.0f) / 2.0f);
      else
         D3DXMatrixTranslation(&translate, 4, 0.0f, ((float) (i+1) * 20.0f) / 2.0f);
		g_pDevice->SetTransform( D3DTS_WORLD, &translate);
		cubemesh[i]->DrawSubset(0);
	}
}

point directXClass::getMouseCoordinates() {
	return point(currentX, currentY);
}

void directXClass::updateCameraTarget()
{
	camera.setvChasePosition(player1.getPositionVector());
	camera.setvChaseDirection(player1.getDirectionVector());
	camera.setvUp(player1.getUpVector());
}

D3DLIGHT9 directXClass::initDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

void directXClass::setupLights()
{
	D3DXVECTOR3 vDir(0.0f, 0.0f, 1.0f);
	D3DXCOLOR c = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight = initDirectionalLight(&vDir, &c);

	g_pDevice->SetLight(0, &dirLight);

	if(dirLightEnabled)
	{
		g_pDevice->LightEnable(0, TRUE);
	}
	else
	{
		g_pDevice->LightEnable(0, FALSE);
	}
}

/*
 Psuedo factory for players - functions like a factory class.
	@params sType
		- Type of ship to create (standard, light, heavy)
	@params pType
		- Type of player to create (human, AI)
	@params pIdx
		- Number of the player (1-8); denotes position within the list of players.
*/
MainPlayerClass directXClass::shipBuilder(EShipType sType, EPlayerType pType, int pIdx, HWND hwnd, WCHAR wszBuff[256], WCHAR wszBuff2[1], WCHAR wszBuff3[2])
{
	switch(sType)
	{
	case STANDARD:
		switch(pType)
		{
		case HUMAN:
			return HumanPlayerClass(
					StandardShipClass(program->g_pMesh, program->g_pMeshMaterials, program->g_pMeshTextures,
					program->g_dwNumMaterials, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
			break;
		case AI:
			      return AIPlayer(
					StandardShipClass(program->g_pMesh, program->g_pMeshMaterials, program->g_pMeshTextures,
					program->g_dwNumMaterials, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));
			
         break;
		}		
		break;
	case LIGHT:
		switch(pType)
		{
		case HUMAN:
			return HumanPlayerClass(
					LightShipClass(program->g_pMeshLight, program->g_pMeshMaterialsLight, program->g_pMeshTexturesLight,
					program->g_dwNumMaterialsLight, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
			break;
		case AI:
			return AIPlayer(
					LightShipClass(program->g_pMeshLight, program->g_pMeshMaterialsLight, program->g_pMeshTexturesLight,
					program->g_dwNumMaterialsLight, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));
			break;
		}

	case HEAVY:
		switch(pType)
		{
		case HUMAN:
			return HumanPlayerClass(
					HeavyShipClass(program->g_pMeshHeavy, program->g_pMeshMaterialsHeavy, program->g_pMeshTexturesHeavy,
					program->g_dwNumMaterialsHeavy, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
			break;
		case AI:
			return AIPlayer(
					HeavyShipClass(program->g_pMeshHeavy, program->g_pMeshMaterialsHeavy, program->g_pMeshTexturesHeavy,
					program->g_dwNumMaterialsHeavy, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));
			break;
		}
	}
}

MainPlayerClass* directXClass::shipBuilder2(EShipType sType, EPlayerType pType, int pIdx, HWND hwnd, WCHAR wszBuff[256], WCHAR wszBuff2[1], WCHAR wszBuff3[2])
{
	switch(sType)
	{
	case STANDARD:
		switch(pType)
		{
		case HUMAN:
			return new HumanPlayerClass(
					StandardShipClass(program->g_pMesh, program->g_pMeshMaterials, program->g_pMeshTextures,
					program->g_dwNumMaterials, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
		case AI:
			      return new AIPlayer(
					StandardShipClass(program->g_pMesh, program->g_pMeshMaterials, program->g_pMeshTextures,
					program->g_dwNumMaterials, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));			
         break;
		}		
		break;
	case LIGHT:
		switch(pType)
		{
		case HUMAN:
			return new HumanPlayerClass(
					LightShipClass(program->g_pMeshLight, program->g_pMeshMaterialsLight, program->g_pMeshTexturesLight,
					program->g_dwNumMaterialsLight, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
			break;
		case AI:
			return new AIPlayer(
					LightShipClass(program->g_pMeshLight, program->g_pMeshMaterialsLight, program->g_pMeshTexturesLight,
					program->g_dwNumMaterialsLight, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));
			break;
		}

	case HEAVY:
		switch(pType)
		{
		case HUMAN:
			return new HumanPlayerClass(
					HeavyShipClass(program->g_pMeshHeavy, program->g_pMeshMaterialsHeavy, program->g_pMeshTexturesHeavy,
					program->g_dwNumMaterialsHeavy, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), (pIdx-1));
			break;
		case AI:
			return new AIPlayer(
					HeavyShipClass(program->g_pMeshHeavy, program->g_pMeshMaterialsHeavy, program->g_pMeshTexturesHeavy,
					program->g_dwNumMaterialsHeavy, g_pDevice),
					std::wstring(wszBuff, 256), _wtoi(wszBuff2), _wtoi(wszBuff3), &program->_gamestate, (pIdx-1));
			break;
		}
      
	}
   return NULL;
}

BOOL CALLBACK directXClass::startDialog (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static int player1ShipClass; //0 = standard 1 = light 2 = heavy
	static int player1TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player2ShipClass; //0 = standard 1 = light 2 = heavy
	static int player2TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player3ShipClass; //0 = standard 1 = light 2 = heavy
	static int player3TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player4ShipClass; //0 = standard 1 = light 2 = heavy
	static int player4TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player5ShipClass; //0 = standard 1 = light 2 = heavy
	static int player5TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player6ShipClass; //0 = standard 1 = light 2 = heavy
	static int player6TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player7ShipClass; //0 = standard 1 = light 2 = heavy
	static int player7TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static int player8ShipClass; //0 = standard 1 = light 2 = heavy
	static int player8TypeAIClosedHuman; //0 = Human 1 = AI 2 = closed
	static MainPlayerClass player1;
	static MainPlayerClass* player2;
	static MainPlayerClass* player3;
	static MainPlayerClass* player4;
	static MainPlayerClass* player5;
	static MainPlayerClass* player6;
	static MainPlayerClass* player7;
	static MainPlayerClass* player8;
	static WCHAR wszBuff[256] = {0};
	static WCHAR wszBuff2[1] = {0};
	static WCHAR wszBuff3[2] = {0};
	switch(msg){
		case WM_CREATE:
			return TRUE;
		case WM_CLOSE:
			ShowWindow(hwnd,SW_HIDE);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					//Setting player 1
					if (player1TypeAIClosedHuman == 0) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO3), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO2), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO1), WM_GETTEXT, 256, (LPARAM)wszBuff2);
						if (player1ShipClass == 0) {
							player1 = shipBuilder(STANDARD, HUMAN, 1, hwnd, wszBuff, wszBuff2, wszBuff3);
							program->currentPlayers[0] = &player1;
						}
						if (player1ShipClass == 1) {
							player1 = shipBuilder(LIGHT, HUMAN, 1, hwnd, wszBuff, wszBuff2, wszBuff3);
							program->currentPlayers[0] = &player1;
						}
						if (player1ShipClass == 2) {
							player1 = shipBuilder(HEAVY, HUMAN, 1, hwnd, wszBuff, wszBuff2, wszBuff3);
							program->currentPlayers[0] = &player1;
						}
						IniPlayerLocation(&player1, 0, 80, -10, 0, 0, 0);
						player1.setShipSpawnLocationRotation(D3DXVECTOR3(0,70,-30),D3DXVECTOR3(0,0,0));
						program->currentPlayers[0] = &player1;
					}
					//Setting player 2
					if (player2TypeAIClosedHuman == 1) {
					   SendMessage(GetDlgItem(hwnd,IDC_COMBO6), WM_GETTEXT, 256, (LPARAM)wszBuff);
					   SendMessage(GetDlgItem(hwnd,IDC_COMBO5), WM_GETTEXT, 256, (LPARAM)wszBuff3);
					   SendMessage(GetDlgItem(hwnd,IDC_COMBO4), WM_GETTEXT, 256, (LPARAM)wszBuff2);
						if (player2ShipClass == 0) {
							player2 = shipBuilder2(STANDARD, AI, 2, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player2ShipClass == 1) {
						    player2 = shipBuilder2(LIGHT, AI, 2, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player2ShipClass == 2) {
							player2 = shipBuilder2(HEAVY, AI, 2, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						IniPlayerLocation(player2, 0, 10, 50, D3DX_PI, 0, 0);
						player2->setShipSpawnLocationRotation(D3DXVECTOR3(0,10,50),D3DXVECTOR3(D3DX_PI,0,0));
						program->currentPlayers[1] = player2;
					}
					if (player2TypeAIClosedHuman == 2) {
						program->currentPlayers[1] = NULL;
					}
					//Setting player 3.
					if (player3TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO9), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO8), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO7), WM_GETTEXT, 256, (LPARAM)wszBuff2);
						if (player3ShipClass == 0) {		
							player3 = shipBuilder2(STANDARD, AI, 3, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player3ShipClass == 1) {
							player3 = shipBuilder2(LIGHT, AI, 3, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player3ShipClass == 2) {
							player3 = shipBuilder2(HEAVY, AI, 3, hwnd, wszBuff, wszBuff2, wszBuff3);
							
						}
						IniPlayerLocation(player3, -30, 10, -40, -D3DX_PI/2, 0, 0);
						player3->setShipSpawnLocationRotation(D3DXVECTOR3(-30,10,-40),D3DXVECTOR3(-D3DX_PI/2,0,0));
						program->currentPlayers[2] = player3;
					}
					if (player3TypeAIClosedHuman == 2) {
						program->currentPlayers[2] = NULL;
					}

					//Setting player 4
					if (player4TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO12), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO11), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO10), WM_GETTEXT, 256, (LPARAM)wszBuff2);
						if (player4ShipClass == 0) {
							player4 = shipBuilder2(STANDARD, AI, 4, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player3ShipClass == 1) {							
							player4 = shipBuilder2(LIGHT, AI, 4, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player4ShipClass == 2) {
							player4 = shipBuilder2(HEAVY, AI, 4, hwnd, wszBuff, wszBuff2, wszBuff3);
							
						}
						IniPlayerLocation(player4, 0, 50, 60, D3DX_PI, 0, 0);
						player4->setShipSpawnLocationRotation(D3DXVECTOR3(0,50,60),D3DXVECTOR3(D3DX_PI,0,0));
						program->currentPlayers[3] = player4;
					}
					if (player4TypeAIClosedHuman == 2) {
						program->currentPlayers[3] = NULL;
					}
					//Setting player 5
					if (player5TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO15), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO14), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO13), WM_GETTEXT, 256, (LPARAM)wszBuff2);

						if (player5ShipClass == 0) {
							player5 = shipBuilder2(STANDARD, AI, 5, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player5ShipClass == 1) {
							player5 = shipBuilder2(LIGHT, AI, 5, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player5ShipClass == 2) {
							player5 = shipBuilder2(HEAVY, AI, 5, hwnd, wszBuff, wszBuff2, wszBuff3);							
						}
						IniPlayerLocation(player5, 30, 10, -40, D3DX_PI/2, 0, 0);
						player5->setShipSpawnLocationRotation(D3DXVECTOR3(30,10,-40),D3DXVECTOR3(D3DX_PI/2,0,0));
						program->currentPlayers[4] = player5;
					}
					if (player5TypeAIClosedHuman == 2) {
						program->currentPlayers[4] = NULL;
					}
					//Setting player 6
					if (player6TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO18), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO17), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO16), WM_GETTEXT, 256, (LPARAM)wszBuff2);
						if (player6ShipClass == 0) {
							player6 = shipBuilder2(STANDARD, AI, 6, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player6ShipClass == 1) {
							player6 = shipBuilder2(LIGHT, AI, 6, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player6ShipClass == 2) {
							player6 = shipBuilder2(HEAVY, AI, 6, hwnd, wszBuff, wszBuff2, wszBuff3);						
						}
						IniPlayerLocation(player6, 60, 10, 30, D3DX_PI/2, 0, 0);
						player5->setShipSpawnLocationRotation(D3DXVECTOR3(60,10,30),D3DXVECTOR3(D3DX_PI/2,0,0));
						program->currentPlayers[5] = player6;                  
					}
					if (player6TypeAIClosedHuman == 2) {
						program->currentPlayers[5] = NULL;
					}
					//Setting player 7
					if (player7TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO21), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO20), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO19), WM_GETTEXT, 256, (LPARAM)wszBuff2);						
						if (player7ShipClass == 0) {							
							player7 = shipBuilder2(STANDARD, AI, 7, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player7ShipClass == 1) {
							player7 = shipBuilder2(LIGHT, AI, 7, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player7ShipClass == 2) {						
							player7 = shipBuilder2(HEAVY, AI, 7, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						IniPlayerLocation(player7, 0, 50, -60, 0, 0, 0);
						player5->setShipSpawnLocationRotation(D3DXVECTOR3(0,50,-60),D3DXVECTOR3(0,0,0));
						program->currentPlayers[6] = player7;
					}
					if (player7TypeAIClosedHuman == 2) {
						program->currentPlayers[6] = NULL;
					}
					//Setting player 8.
					if (player8TypeAIClosedHuman == 1) {
						SendMessage(GetDlgItem(hwnd,IDC_COMBO24), WM_GETTEXT, 256, (LPARAM)wszBuff);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO23), WM_GETTEXT, 256, (LPARAM)wszBuff3);
						SendMessage(GetDlgItem(hwnd,IDC_COMBO22), WM_GETTEXT, 256, (LPARAM)wszBuff2);						
					if (player8ShipClass == 0) {
							player8 = shipBuilder2(STANDARD, AI, 8, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player8ShipClass == 1) {
							player8 = shipBuilder2(LIGHT, AI, 8, hwnd, wszBuff, wszBuff2, wszBuff3);
						}
						if (player8ShipClass == 2) {
							player8 = shipBuilder2(HEAVY, AI, 8, hwnd, wszBuff, wszBuff2, wszBuff3);						
						}
						IniPlayerLocation(player8, -60, 10, 30, -D3DX_PI/2, 0, 0);
						player5->setShipSpawnLocationRotation(D3DXVECTOR3(-60, 10, 30),D3DXVECTOR3(-D3DX_PI/2,0,0));
						program->currentPlayers[7] = player8;
					}
					if (player8TypeAIClosedHuman == 2) {
						program->currentPlayers[7] = NULL;
					}

					//Setting projectiles.
					for (int i = 0; i < 8; i++)
					{
						if (program->currentPlayers[i] != NULL) {
							program->currentPlayers[i]->initProjectiles(program->g_pMeshLaser, program->g_pMeshMaterialsLaser, program->g_pMeshTexturesLaser, program->g_dwNumMaterialsLaser);
						}
					}
					program->player1 = *program->currentPlayers[0];
					program->currentPlayers[0] = &program->player1;
					program->_gamestate.updateGameState(program->g_hWndMain, program->currentPlayers);
					program->menuSelect = 2;
					program->updateCameraTarget();
					program->camera.reset();
					ShowWindow(hwnd,SW_HIDE);
					return TRUE;
				case IDCANCEL:
					ShowWindow(hwnd,SW_HIDE);
					return TRUE;
				case IDC_RADIO1:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO1), BM_CLICK, NULL, NULL);
					player1TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO2:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO2), BM_CLICK, NULL, NULL);
					player1TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO3:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO3), BM_CLICK, NULL, NULL);
					player1TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO4:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO4), BM_CLICK, NULL, NULL);
					player1ShipClass = 0;
					return TRUE;
				case IDC_RADIO5:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO5), BM_CLICK, NULL, NULL);
					player1ShipClass = 1;
					return TRUE;
				case IDC_RADIO6:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO6), BM_CLICK, NULL, NULL);
					player1ShipClass = 2;
					return TRUE;
				case IDC_RADIO7:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO7), BM_CLICK, NULL, NULL);
					player2TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO8:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO8), BM_CLICK, NULL, NULL);
					player2TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO9:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO9), BM_CLICK, NULL, NULL);
					player2TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO10:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO10), BM_CLICK, NULL, NULL);
					player2ShipClass = 0;
					return TRUE;
				case IDC_RADIO11:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO11), BM_CLICK, NULL, NULL);
					player2ShipClass = 1;
					return TRUE;
				case IDC_RADIO12:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO12), BM_CLICK, NULL, NULL);
					player2ShipClass = 2;
					return TRUE;
				case IDC_RADIO13:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO13), BM_CLICK, NULL, NULL);
					player3TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO14:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO14), BM_CLICK, NULL, NULL);
					player3TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO15:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO15), BM_CLICK, NULL, NULL);
					player3TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO16:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO16), BM_CLICK, NULL, NULL);
					player3ShipClass = 0;
					return TRUE;
				case 1040:
					SendMessage(GetDlgItem(hwnd,1040), BM_CLICK, NULL, NULL);
					player3ShipClass = 1;
					return TRUE;
				case IDC_RADIO18:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO18), BM_CLICK, NULL, NULL);
					player3ShipClass = 2;
					return TRUE;
				case IDC_RADIO19:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO19), BM_CLICK, NULL, NULL);
					player4TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO20:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO20), BM_CLICK, NULL, NULL);
					player4TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO21:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO21), BM_CLICK, NULL, NULL);
					player4TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO22:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO22), BM_CLICK, NULL, NULL);
					player4ShipClass = 0;
					return TRUE;
				case IDC_RADIO23:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO23), BM_CLICK, NULL, NULL);
					player4ShipClass = 1;
					return TRUE;
				case IDC_RADIO24:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO24), BM_CLICK, NULL, NULL);
					player4ShipClass = 2;
					return TRUE;
				case IDC_RADIO25:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO25), BM_CLICK, NULL, NULL);
					player5TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO26:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO26), BM_CLICK, NULL, NULL);
					player5TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO27:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO27), BM_CLICK, NULL, NULL);
					player5TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO28:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO28), BM_CLICK, NULL, NULL);
					player5ShipClass = 0;
					return TRUE;
				case IDC_RADIO29:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO29), BM_CLICK, NULL, NULL);
					player5ShipClass = 1;
					return TRUE;
				case IDC_RADIO30:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO30), BM_CLICK, NULL, NULL);
					player5ShipClass = 2;
					return TRUE;
				case IDC_RADIO31:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO31), BM_CLICK, NULL, NULL);
					player6TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO32:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO32), BM_CLICK, NULL, NULL);
					player6TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO33:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO33), BM_CLICK, NULL, NULL);
					player6TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO34:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO34), BM_CLICK, NULL, NULL);
					player6ShipClass = 0;
					return TRUE;
				case IDC_RADIO35:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO35), BM_CLICK, NULL, NULL);
					player6ShipClass = 1;
					return TRUE;
				case IDC_RADIO36:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO36), BM_CLICK, NULL, NULL);
					player6ShipClass = 2;
					return TRUE;
				case IDC_RADIO37:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO37), BM_CLICK, NULL, NULL);
					player7TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO38:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO38), BM_CLICK, NULL, NULL);
					player7TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO39:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO39), BM_CLICK, NULL, NULL);
					player7TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO40:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO40), BM_CLICK, NULL, NULL);
					player7ShipClass = 0;
					return TRUE;
				case IDC_RADIO41:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO41), BM_CLICK, NULL, NULL);
					player7ShipClass = 1;
					return TRUE;
				case IDC_RADIO42:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO42), BM_CLICK, NULL, NULL);
					player7ShipClass = 2;
					return TRUE;
				case IDC_RADIO43:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO43), BM_CLICK, NULL, NULL);
					player8TypeAIClosedHuman = 0;
					return TRUE;
				case IDC_RADIO44:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO44), BM_CLICK, NULL, NULL);
					player8TypeAIClosedHuman = 1;
					return TRUE;
				case IDC_RADIO45:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO45), BM_CLICK, NULL, NULL);
					player8TypeAIClosedHuman = 2;
					return TRUE;
				case IDC_RADIO46:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO46), BM_CLICK, NULL, NULL);
					player8ShipClass = 0;
					return TRUE;
				case IDC_RADIO47:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO47), BM_CLICK, NULL, NULL);
					player8ShipClass = 1;
					return TRUE;
				case IDC_RADIO48:
					SendMessage(GetDlgItem(hwnd,IDC_RADIO48), BM_CLICK, NULL, NULL);
					player8ShipClass = 2;
					return TRUE;
			}
		default:
			return FALSE;
	}
	return FALSE;
}

// Initializes location, rotation and movement bounds for player.
// param px, py, pz: (x, y, z) position of player.
// param rx, ry, rz: (x, y, z) rotation of player.
void directXClass::IniPlayerLocation(MainPlayerClass* player, float px, float py, float pz,
   float rx, float ry, float rz)
{
   player->GetShip()->SetPosition(px, py, pz);
	player->GetShip()->SetRotation(rx, ry, rz);
	player->GetShip()->Update(2);
	player->GetShip()->SetRotation(0, 0, 0);
	player->SetBounds(NULL);
}

// Initializes location, rotation and movement bounds for player. to a default start location
// param px, py, pz: (x, y, z) position of player.
// param rx, ry, rz: (x, y, z) rotation of player.
void directXClass::RespawnPlayer(MainPlayerClass* player)
{
	player->respawn();
	player->GetShip()->Update(2);
	player->GetShip()->SetRotation(0, 0, 0);
	player->SetBounds(NULL);
}