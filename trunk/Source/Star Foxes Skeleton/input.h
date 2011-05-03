#ifndef INPUT_H
#define INPUT_H

#include "directXHeader.h"

class Input
{
private:
	LPDIRECTINPUT8 keyb; // pointer to DirectInput interface
	LPDIRECTINPUTDEVICE8 keybdev; // the pointer to the keyboard device
	BYTE keystate[256]; // stores keystroke information
	
public:
	void init_keyboard(HWND hWnd); // sets up and inilializes the keyboard
	void read_keyboard(void); // reads and responds to keyboard input
	void clean_input(void); // cleans up direct input
	
	// returns true if a key has been pressed 
	// eg. get_keystate(DIK_UP) returns true if up has been pressed
	inline BOOL get_keystate(int value){return (keystate[value] & 0x80);}
};

#endif