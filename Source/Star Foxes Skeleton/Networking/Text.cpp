#include "Text.h"

// -----------------------------------------------------------------
// Draws the text to the device specified when Text instantiated.
// -----------------------------------------------------------------
// param str: the string to draw on device.
// -----------------------------------------------------------------
void Text::draw(const char* str)
{
   // Draw some text 
   m_font->DrawTextA(NULL, str, -1, &_rct, _format, _fontColor );
}

void Text::draw(const char* str, RECT* rct)
{
   // Draw some text 
   m_font->DrawTextA(NULL, str, -1, rct, _format, _fontColor );
}

// ----------------------------------------------------------------------
// Create a rectangle to indicate where on the screen it should be drawn.   
// ----------------------------------------------------------------------
// param x: x position.
// param y: y position.
// param w: width.
// param h: height.
// ----------------------------------------------------------------------
void Text::setRect(int x, int y, int w, int h)
{
   SetRect(&_rct, x, y, w, h);
}

// ----------------------------------------------------------------------
// Sets color of the text.
// ----------------------------------------------------------------------
// param R: the r value.
// param G: the g value.
// param B: the b value.
// param A: the opacity value.
// ----------------------------------------------------------------------
void Text::setColor(int R, int G, int B, int A)
{
   //_fontColor = D3DCOLOR_ARGB(255,0,0,120);
   _fontColor = D3DCOLOR_XRGB(R, G, B);
}

// ----------------------------------------------------------------------
// Constructor: initializes the text with the device.
// ----------------------------------------------------------------------
// param: the device to be drawn to.
// ----------------------------------------------------------------------
Text::Text(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft, SIZE charsz)
{
   IniText(gD3dDevice, rightToLeft, charsz);
}

Text::Text(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft)
{
   SIZE charsize;
   charsize.cx = 30;
   charsize.cy = 35;
   IniText(gD3dDevice, rightToLeft, charsize);
}

Text::Text(LPDIRECT3DDEVICE9 gD3dDevice)
{
   SIZE charsize;
   charsize.cx = 30;
   charsize.cy = 35;
   IniText(gD3dDevice, false, charsize);
}

void Text::IniText(LPDIRECT3DDEVICE9 gD3dDevice, bool rightToLeft, SIZE charsz)
{
   //_rightToLeft = rightToLeft;
   if(rightToLeft)
      _format = DT_RIGHT|DT_NOCLIP;
   else
      _format = DT_NOCLIP;

   D3DXCreateFont(gD3dDevice, charsz.cy, charsz.cx, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
      TEXT("Arial"), &m_font );
   setRect(100, 100, 500, 500);
   setColor(255, 255, 255, 255);
}

// ----------------------------------------------------------------------
// Constructor: Default
// ----------------------------------------------------------------------
Text::Text(void)
{
}

// ----------------------------------------------------------------------
// Destructor: Releases the resources of the font.
// ----------------------------------------------------------------------
Text::~Text(void)
{
   m_font->Release();
}
