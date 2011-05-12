#pragma once

#include "../directXHeader.h"

// ----------------------------------------
// Represents text to be drawn on screen.
// ----------------------------------------
class Text
{
private:

   // the fonts of the text.
   ID3DXFont *m_font;
   
   // colour for the text.
   D3DCOLOR _fontColor;

   // rectangle where text is to be drawn.
   RECT _rct;

   DWORD _format;
    
public:
   void IniText(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft, SIZE charsize);

   // function to set text rectangle.
   void setRect(int x, int y, int w, int h);

   // function to set text color.
   void setColor(int R, int G, int B, int A);

   // --------------------------------------------------------
   // Description of all the following functions please refer
   // to the functions definition in the Text.cpp file.
   // --------------------------------------------------------
   void draw(const char* str);
   Text(LPDIRECT3DDEVICE9 gD3dDevice);
   Text(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft, SIZE charsz);
   Text(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft);
   void draw(const char* str, RECT* rct);
   Text(void);
   ~Text(void);
};
