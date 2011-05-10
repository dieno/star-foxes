#include "directXHeader.h"
#include "gamecore.h"

// sets up an interface to the keyboard
void Input::init_keyboard(HWND hWnd)
{
	// creates a direct input object
    DirectInput8Create(GetModuleHandle(NULL), 
					   DIRECTINPUT_VERSION, 
					   IID_IDirectInput8, 
					   (void**)&keyb, 
					   NULL);

	// creates the keyboard device
    keyb->CreateDevice(GUID_SysKeyboard, &keybdev, NULL);
 
    keybdev->SetDataFormat(&c_dfDIKeyboard);
    keybdev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

// reads and responds to keyboard input
void Input::read_keyboard(void)
{
	// get access if we don't have it already
    keybdev->Acquire();

    // get the input data
    keybdev->GetDeviceState(256, (LPVOID)keystate);
}

// closes and cleans DirectInput
void Input::clean_input(void)
{
	keybdev->Unacquire();    // make sure the keyboard is unacquired
    keyb->Release();    // close DirectInput before exiting
}
