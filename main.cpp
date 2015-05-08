#define DIRECTINPUT_VERSION 0x0800
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#include <cstdlib>
#include <sstream>

#include "resource.h"
#include "windows.h"
#include "d3d9.h" 
#include "d3dx9.h"
#include "dinput.h" 

#include "game.h"
#include "multiplayerGame.h"
#include "ui_panel.h"
#include "ui_button.h"
#include "ui_label.h"

using Tetris::Color;
using Tetris::Game;
using Tetris::Tetromino;

// Constants
const char	MAIN_WINDOW_TITLE[] = "Tetris",
			MAIN_WINDOW_CLASS_NAME[] = "WindowClass";

const int	CLIENT_AREA_WIDTH  = 640,
			CLIENT_AREA_HEIGHT = 480;

// Globals
const unsigned int GAME_START = WM_USER + 1;
const unsigned int GAME_RESUME = WM_USER + 2;
bool fullscreen = false;
//TODO: move these out of global
//DirectX
bool deviceLost = false;
LPDIRECT3D9 directX;
LPDIRECT3DDEVICE9 directXDevice;
LPDIRECTINPUTDEVICE8 keyboardDevice = NULL;
LPD3DXFONT scoreFont;
// mesh declarations
LPD3DXMESH meshBlock;    
LPD3DXMESH meshBoard;   
D3DMATERIAL9 blockMaterial;
//Game
UI::Panel* menuPtr = NULL;
Game* gamePtr = NULL;
//Thread handels
HANDLE gameThreadHandle;
HANDLE drawThreadHandle;
HANDLE gameMutex;
HANDLE menuMutex;


void initGame()
{
	WaitForSingleObject(gameMutex, INFINITE);
		SAFE_DELETE(gamePtr);
		gamePtr = new Game();
	ReleaseMutex(gameMutex);
}

void initMenu()
{
	const int fontSize(25);
	const int buttonWidth(250);
	const Color fontColor(160,160,160);	
	const int posX = (CLIENT_AREA_WIDTH/2) - (buttonWidth/2);
	const int posY = (CLIENT_AREA_HEIGHT/2);
	WaitForSingleObject(menuMutex, INFINITE);			
		menuPtr = new UI::Panel( 
			static_cast<float>(CLIENT_AREA_HEIGHT) * 0.75f,  
			static_cast<float>(CLIENT_AREA_HEIGHT) * 0.75f, 
			Point2D<float>(CLIENT_AREA_WIDTH/2, CLIENT_AREA_HEIGHT/2), 
			directXDevice
			);
		
		//add buttons to the menu
		if (gamePtr->GetState() == Game::Paused)
		{
			menuPtr->AddChild( new UI::Button("RESUME GAME", posX, posY + (fontSize*2) * menuPtr->Count(), buttonWidth, fontSize, fontColor, GAME_RESUME, directXDevice) );
		}
		menuPtr->AddChild( new UI::Button("NEW GAME", posX, posY + (fontSize*2) * menuPtr->Count(), buttonWidth, fontSize, fontColor, GAME_START, directXDevice) );
		menuPtr->AddChild( new UI::Button("QUIT", posX, posY + (fontSize*2) * menuPtr->Count(), buttonWidth, fontSize, fontColor, WM_CLOSE, directXDevice) );

		//add a title to the menu
		switch (gamePtr->GetState())
		{
			case Game::Paused:
				menuPtr->AddChild( new UI::Label("(Paused)", posX, posY - 50, buttonWidth, fontSize, fontColor, directXDevice) );				
			break;
			case Game::Over:
				menuPtr->AddChild( new UI::Label("(Game Over)", posX, posY - 50, buttonWidth, fontSize, fontColor, directXDevice) );
			break;
		}
	ReleaseMutex(menuMutex);
}



bool initDevice(HWND hWnd){
	
	D3DPRESENT_PARAMETERS dxPresentParams;
	ZeroMemory( &dxPresentParams, sizeof(dxPresentParams) );
	dxPresentParams.Windowed = !fullscreen;
	dxPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dxPresentParams.BackBufferFormat = D3DFMT_UNKNOWN;		
    dxPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dxPresentParams.hDeviceWindow = hWnd;
    dxPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
    dxPresentParams.BackBufferWidth = CLIENT_AREA_WIDTH;
    dxPresentParams.BackBufferHeight = CLIENT_AREA_HEIGHT;
    dxPresentParams.EnableAutoDepthStencil = TRUE;  
    dxPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

	if (directX == NULL){
		directX = Direct3DCreate9(D3D_SDK_VERSION);
		if (directX == NULL){
			::MessageBox(hWnd, "DirectX runtime library is not installed.", "System error", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	//multi sampling
	D3DDISPLAYMODE d3ddm;
	directX->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);	
	DWORD total = 0;
	if(SUCCEEDED(directX->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, true, D3DMULTISAMPLE_NONMASKABLE, &total)))
	{
		dxPresentParams.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		dxPresentParams.MultiSampleQuality = total - 1;
	}
	
	if (deviceLost)
	{
		if ( FAILED(directXDevice->Reset(&dxPresentParams)) )
		{
			return false;
		}
	}
	else if (FAILED(directX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dxPresentParams, &directXDevice)))
	{
		if (FAILED(directX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dxPresentParams, &directXDevice)))
		{
			::MessageBox(hWnd, "CreateDevice() failed", "System error", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
	}
	directXDevice->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
    directXDevice->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
    directXDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light
	return true;
}

void initCamera(){
	D3DXVECTOR3 eye(10, 20, -15);
	D3DXVECTOR3 target(0, 10, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMATRIXA16 view;
	D3DXMatrixLookAtLH(&view, &eye, &target, &up);
	
	const float FOV = 45; //D3DX_PI*0.25f;
	D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection, FOV, static_cast<float>(CLIENT_AREA_WIDTH)/static_cast<float>(CLIENT_AREA_HEIGHT), 1, 500);
	
	if (
		FAILED(directXDevice->SetTransform(D3DTS_VIEW, &view)) ||
		FAILED(directXDevice->SetTransform(D3DTS_PROJECTION, &mProjection)) ||
		FAILED(directXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID))
		)
	{
		throw UI::InitException("Error Initializing Camera.");
	}
}


void initFont()
{	
	if (FAILED(D3DXCreateFont( directXDevice, 25, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &scoreFont)))
	{
		throw UI::InitException("Error Initializing Font.");
	}
}


void initLight()
{
    D3DLIGHT9 light;

    ZeroMemory(&light, sizeof(light));
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Diffuse.a = 1.0f;

    D3DVECTOR vecDirection = {-5.0f, -8.0f, 10.0f};
    light.Direction = vecDirection;

	if ( FAILED(directXDevice->SetLight(0, &light)) || FAILED(directXDevice->LightEnable(0, TRUE)) )
	{ 
		throw UI::InitException("Error Initializing light.");
	}
}


void initMesh()
{
	if (FAILED(D3DXLoadMeshFromXResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MODEL_BLOCK), RT_RCDATA, D3DXMESH_SYSTEMMEM, directXDevice, NULL, NULL, NULL, NULL, &meshBlock)) || 	
		FAILED(D3DXLoadMeshFromXResource(GetModuleHandle(NULL),  MAKEINTRESOURCE(IDR_MODEL_BOARD), RT_RCDATA, D3DXMESH_SYSTEMMEM, directXDevice, NULL, NULL, NULL, NULL, &meshBoard)) )
	{
		throw UI::InitException("Error Initializing model.");
	}
}



void SetMaterialColor(D3DMATERIAL9 * mat, float r,float g, float b, float alpha = 1.0)
{
	ZeroMemory(mat, sizeof(D3DMATERIAL9));
	mat->Diffuse.r = mat->Ambient.r = r;
	mat->Diffuse.g = mat->Ambient.g = g;
	mat->Diffuse.b = mat->Ambient.b = b;
	mat->Diffuse.a = mat->Ambient.a = alpha;
	mat->Specular.b = mat->Specular.g = mat->Specular.r = 1.0f;
	mat->Power = 50.0f;	
}

void drawBlock(float x, float y, float z, const Color& c)
{
	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix, x, y, z );
	directXDevice->SetTransform(D3DTS_WORLD, &matrix);
	SetMaterialColor(&blockMaterial, static_cast<float>(c.GetRed()) / 255, static_cast<float>(c.GetGreen()) / 255, static_cast<float>(c.GetBlue()) / 255);
	directXDevice->SetMaterial(&blockMaterial);
	meshBlock->DrawSubset(0);
}

void drawScore()
{
	if (scoreFont)
	{
		std::ostringstream scoreStringStream;
		WaitForSingleObject(gameMutex, INFINITE);	
		scoreStringStream 
			<< "Score: " << gamePtr->GetScore() << std::endl 
			<< "Level: " << gamePtr->GetLevel() << std::endl 
			<< "Lines: " << gamePtr->GetTotalLines() << std::endl;
		ReleaseMutex(gameMutex);

		// Create a rectangle to indicate where on the screen it should be drawn
		RECT fontRectangle;
		SetRect( &fontRectangle, 20, 20, CLIENT_AREA_WIDTH - 20, CLIENT_AREA_HEIGHT - 20);
		scoreFont->DrawText(NULL, scoreStringStream.str().c_str(), -1, &fontRectangle, DT_TOP, D3DCOLOR_XRGB(160, 160, 160) );
	}
}

void drawScene()
{	 
	WaitForSingleObject(gameMutex, INFINITE);
	D3DXMATRIX matrix;

	float offsetX = -5;
	float offsetY = 0;
	float offsetZ = 0;

	//draw board
	D3DXMatrixTranslation(&matrix, offsetX+4.5f, offsetY, offsetZ );
	directXDevice->SetTransform(D3DTS_WORLD, &matrix);
	SetMaterialColor(&blockMaterial, 0.995f, 0.995f, 0.995f);
	directXDevice->SetMaterial(&blockMaterial);
	meshBoard->DrawSubset(0);

	//draw static blocks
	for(int x = 0; x < gamePtr->GetBoardWidth(); x++)
	{
		for(int y = 0; y < gamePtr->GetBoardHeight(); y++)
		{
			if (!gamePtr->GetBoardPosition(x, y).IsEmpty())
			{
				drawBlock(
					static_cast<float>(x) + offsetX, 
					static_cast<float>(y) + offsetY, 
					offsetZ, 
					gamePtr->GetBoardPosition(x, y)
				);
			}
		}
	}
 
	//draw current tetrimino
	for(int x = 0; x < Tetromino::BlockCount; x++)
	{	
		drawBlock(
			static_cast<float>(gamePtr->GetCurrentBlockX(x)) + offsetX, 
			static_cast<float>(gamePtr->GetCurrentBlockY(x)) + offsetY, 
			offsetZ, 
			gamePtr->GetCurrentColor()
		);
	}

	offsetX = -15;
	offsetY = 0;
	offsetZ = 0;

	//draw next tetrimino
	for(int x = 0; x < Tetromino::BlockCount; x++)
	{	
		drawBlock(
			static_cast<float>(gamePtr->GetNextBlockX(x)) + offsetX, 
			static_cast<float>(gamePtr->GetNextBlockY(x)) + offsetY, 
			offsetZ, 
			gamePtr->GetNextColor()
		);
	}
	ReleaseMutex(gameMutex);

	//draw score
	drawScore();

	
}




void drawMenu(HWND hwnd)
{
	if (gamePtr->GetState() != Game::Running)
	{
		if (menuPtr != NULL)
		{
			WaitForSingleObject(menuMutex, INFINITE);
				POINT mousePositionPoint;	
				if (GetCursorPos(&mousePositionPoint) != 0)
				{ 
					ScreenToClient(hwnd, &mousePositionPoint); 
				}
				menuPtr->Draw(directXDevice, mousePositionPoint.x, mousePositionPoint.y);
			ReleaseMutex(menuMutex);
		}				
	}
}

LPDIRECTINPUTDEVICE8 initKeyboard(HWND hwnd)
 {
     LPDIRECTINPUT8 p_dx_KeybObject;
     LPDIRECTINPUTDEVICE8 p_dx_KeybDevice;
 
     DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&p_dx_KeybObject, NULL);
     p_dx_KeybObject->CreateDevice(GUID_SysKeyboard, &p_dx_KeybDevice, NULL);
 
     p_dx_KeybDevice->SetDataFormat(&c_dfDIKeyboard);
     p_dx_KeybDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);

	 //buffered input
	DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 8;
	p_dx_KeybDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

    p_dx_KeybDevice->Acquire(); 
    return p_dx_KeybDevice;
 }

HRESULT ReadKeyboardBuffer(HWND hDlg)
{
	const int SAMPLE_BUFFER_SIZE = 8;
    DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data 
    
    HRESULT hr;

    if(keyboardDevice == NULL)
	{
        return S_OK;
	}

    DWORD dwElements = SAMPLE_BUFFER_SIZE;
    hr = keyboardDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
    if( hr != DI_OK ) 
    {
        // error / DI_BUFFEROVERFLOW.
        hr = keyboardDevice->Acquire();

        while(hr == DIERR_INPUTLOST)
		{
            hr = keyboardDevice->Acquire();
		}

        if(hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED) 
		{
			return 0; 
		}
    }

	//process buffered keyboard input
    for( DWORD i = 0; i < dwElements; i++ ) 
    {	
		if ((didod[ i ].dwData & 0x80)) //keyup
		{
			
			WaitForSingleObject(gameMutex, INFINITE);

			if (gamePtr->GetState() == Game::Running)
			{

				switch (didod[ i ].dwOfs)
				{
				
				case DIK_ESCAPE:
				
					if (gamePtr->GetState() == Game::Running)
					{
						gamePtr->SetState(Game::Paused);
					}
					else if (gamePtr->GetState() == Game::Paused)
					{
						gamePtr->SetState(Game::Running);
						SAFE_DELETE(menuPtr);
					}
					break;

				case DIK_LEFT:
					gamePtr->MoveLeft();
					break;

				case DIK_RIGHT:
					gamePtr->MoveRight();
					break;

				case DIK_DOWN:		
					gamePtr->MoveDown();
					break;

				case DIK_UP:		
					gamePtr->Rotate();			  
					break;

				case DIK_SPACE:
					gamePtr->Drop();
					break;
				}
			}
			else if (gamePtr->GetState() == Game::Paused)
			{
				switch (didod[ i ].dwOfs)
				{
				
				case DIK_ESCAPE:
					gamePtr->SetState(Game::Running);
					SAFE_DELETE(menuPtr); //TODO: mutex here
					break;
				}
			}	
			ReleaseMutex(gameMutex);

		}
    }
    return S_OK;
}





DWORD WINAPI gameThread(LPVOID pvoid)
{
	while(true)
	{
		WaitForSingleObject(gameMutex, INFINITE);
		const Game::State currentState = gamePtr->GetState();		
		ReleaseMutex(gameMutex);

		if(currentState == Game::Running)
		{
			
			WaitForSingleObject(gameMutex, INFINITE);
			if (!gamePtr->CheckCollision(0, -1))
			{			
  				gamePtr->MoveDown();
			}
			else
			{
				gamePtr->NewTetromino();
			}
			ReleaseMutex(gameMutex);
			Sleep(gamePtr->GetSpeed());
		}
		else
		{
			Sleep(1000);
		}
		
	}
	return 0;
}

DWORD WINAPI inputThread(LPVOID pvoid)
{
	
	while(true)
	{
		WaitForSingleObject(gameMutex, INFINITE);
		Game::State currentState = gamePtr->GetState();		
		ReleaseMutex(gameMutex);

		switch (currentState)
		{
			case Game::Over:
				Sleep(1000);
				break;
			case Game::Paused:
				//processKeyboardMenu();
				break;
			case Game::Running:
				//processKeyboard();
				break;
		}			
	}
	return 0;
}


DWORD WINAPI drawThread(HWND hwnd)
{
	while(true)
	{
		if (!deviceLost)
		{
			//reinit menu if needed
			if ((gamePtr->GetState() != Game::Running) && (menuPtr == NULL))
			{
				initMenu();
			}

			//WaitForSingleObject(menuMutex, INFINITE);			
			//	bool redrawMenu = (menuPtr != NULL) ?  menuPtr->IsDirty() : false;	
			//ReleaseMutex(menuMutex);

			//WaitForSingleObject(gameMutex, INFINITE);
			//	const bool redrawGame = (gamePtr != NULL) ?  gamePtr->IsDirty() : false;
			//ReleaseMutex(gameMutex);

		
			bool redrawMenu = true;	
			const bool redrawGame = true;
			

			if (redrawGame || redrawMenu)
			{
				directXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
				directXDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				directXDevice->BeginScene();

				drawScene();
				drawMenu(hwnd);
				
				directXDevice->EndScene();
				if (directXDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
				{
					deviceLost = true;				
				}
				else
				{
					gamePtr->ClearDirty();
				}
			}
		}
		
		Sleep(10);
	}
	return 0;
}

void cleanup()
{	
	SAFE_RELEASE(meshBlock);
	SAFE_RELEASE(meshBoard);
	SAFE_RELEASE(scoreFont);
	SAFE_RELEASE(keyboardDevice);
	SAFE_RELEASE(directXDevice);
	SAFE_RELEASE(directX)
	SAFE_DELETE(gamePtr);
	SAFE_DELETE(menuPtr);
	
}


void closeThread(HANDLE threadHandle)
{
	//TODO: fix this function
	DWORD exitCode;
	GetExitCodeThread(threadHandle, &exitCode); 
	DWORD status = 0;
	//if thread isn't finished wait for it to exit
	while ((status == 0) && (exitCode == STILL_ACTIVE))
	{
		TerminateThread(threadHandle, status);
		Sleep(50);
		GetExitCodeThread(threadHandle, &exitCode); 
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_ACTIVATEAPP:
			if (deviceLost)
			{
				if (directXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				{
					SAFE_RELEASE(scoreFont);
					SAFE_DELETE(menuPtr);
					if (initDevice(hwnd))
					{
						initLight();
						initCamera();
						initFont();
						deviceLost = false;
					}
				}
			}
			break;
		case WM_LBUTTONUP:
			WaitForSingleObject(gameMutex, INFINITE);
			if (gamePtr->GetState() != Game::Running)
			{
				if (!FAILED(WaitForSingleObject(menuMutex, INFINITE)))
				{
				menuPtr->MouseClick(LOWORD(lParam), HIWORD(lParam), hwnd);
				ReleaseMutex(menuMutex);
				}
			}
			ReleaseMutex(gameMutex);
		break;
		case GAME_START:
			WaitForSingleObject(gameMutex, INFINITE);
				initGame();
			ReleaseMutex(gameMutex);
			if (!FAILED(WaitForSingleObject(menuMutex, INFINITE)))
			{
				SAFE_DELETE(menuPtr);
				ReleaseMutex(menuMutex);
			}

		break;
		case GAME_RESUME:
			if (!FAILED(WaitForSingleObject(menuMutex, INFINITE)))
			{
				SAFE_DELETE(menuPtr);
				ReleaseMutex(menuMutex);
			}
			WaitForSingleObject(gameMutex, INFINITE);
				gamePtr->SetState(Game::Running);
			ReleaseMutex(gameMutex);
		break;
		case WM_TIMER:
			if( FAILED(ReadKeyboardBuffer(hwnd)) )
			{
				//TODO: msgbox error message
				KillTimer( hwnd, 0 );  
			}
			break;


		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}



void initWindow(HINSTANCE hInstance, HWND* hwnd, WNDCLASSEX& w)
{
	//Register window class
	w.cbSize = sizeof(WNDCLASSEX);
	w.style = 0;
	w.lpfnWndProc = WndProc;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hInstance = hInstance;
	w.hIcon =  LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	w.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.lpszMenuName = NULL;
	w.lpszClassName = MAIN_WINDOW_CLASS_NAME;
	
	if(!RegisterClassEx(&w)){
		MessageBox(NULL, "RegisterClassEx() failed", "System error",
			MB_ICONEXCLAMATION | MB_OK);
	}

	DWORD windowStyle = WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION;
	if (fullscreen)
	{
		 windowStyle = WS_EX_TOPMOST | WS_POPUP;
	}
	
	RECT windowRect;
	SetRect(&windowRect, 0, 0, CLIENT_AREA_WIDTH, CLIENT_AREA_HEIGHT);
	AdjustWindowRect(&windowRect, windowStyle, false);

	// Create window
	*hwnd = CreateWindowEx(
		NULL,
		MAIN_WINDOW_CLASS_NAME,
		MAIN_WINDOW_TITLE,
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		abs(windowRect.left - windowRect.right), 
		abs(windowRect.top - windowRect.bottom),
		NULL, NULL, hInstance, NULL);	
	
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	HWND hwnd = NULL;
	MSG msg;
	WNDCLASSEX w;

	initWindow(hInstance, &hwnd, w);	

	if(hwnd == NULL){
		MessageBox(NULL, "Creating Window failed", "System error", MB_OK | MB_ICONERROR);
		return 0;
	}


	// Show window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Init DirectX objects
	try
	{
		initDevice(hwnd);
		initLight();
		initCamera();
		initMesh();
		initFont();
		keyboardDevice = initKeyboard(hwnd);
	}
	catch(UI::InitException ex)
	{
		::MessageBox(hwnd, ex.GetExtraInfo().c_str(), "Error", MB_OK | MB_ICONERROR);
		cleanup();
		DestroyWindow(hwnd);
		return -1;
	}
	
	

	SetTimer( hwnd, 0, 1000 / 500, NULL );

	// Create mutex for multithread object locking
	gameMutex = CreateMutex( NULL, false, NULL);
	menuMutex = CreateMutex( NULL, false, NULL);

	if ((gameMutex == NULL)  || (menuMutex == NULL))
    {
        ::MessageBox(hwnd, "ERROR Creating mutex", "ERROR", MB_OK | MB_ICONERROR);
        return 0;
    }

	initGame();
	gamePtr->SetState(Game::Over);
	initMenu();

	DWORD gameThreadID;
	gameThreadHandle = CreateThread(NULL, 0, gameThread, (LPVOID)hwnd, 0, &gameThreadID);
	
	DWORD drawThreadID;
	drawThreadHandle = CreateThread(NULL, 0,  (LPTHREAD_START_ROUTINE)drawThread, (LPVOID)hwnd, 0, &drawThreadID);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	
	closeThread(drawThreadHandle);
	closeThread(gameThreadHandle);
	CloseHandle(gameMutex);
	CloseHandle(menuMutex);
	
	cleanup();
	DestroyWindow(hwnd);
	return 0;
}

