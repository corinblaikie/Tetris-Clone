#include "ui_button.h"


UI::Button::Button(string text, long positionX, long positionY, long width, long height,  Tetris::Color c, UINT callbackMessage, LPDIRECT3DDEVICE9 directXDevice)
{
	// Create a rectangle to indicate where on the screen it should be drawn	
	buttonRect.left = positionX;
	buttonRect.right = positionX + width;
	buttonRect.top = positionY;
	buttonRect.bottom = positionY + height;
	buttonText = text;
	fontColor = c;
	fontColorHover = Tetris::Color(255,255,0);
	msg = callbackMessage;
	buttonFont = NULL;
	D3DXCreateFont( directXDevice, height, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &buttonFont );
}

bool UI::Button::CheckCollision(int x, int y)
{
	return ((y < buttonRect.bottom) && (y > buttonRect.top) && (x > buttonRect.left) && (x < buttonRect.right));
}

void UI::Button::MouseClick(int x, int y, HWND hwnd)
{
	if ((y < buttonRect.bottom) && (y > buttonRect.top) && (x > buttonRect.left) && (x < buttonRect.right))
	{
		::PostMessage(hwnd, msg, 0 , 0);
	}
}


void UI::Button::Draw(LPDIRECT3DDEVICE9 directXDevice, int mouseX, int mouseY)
{
	Tetris::Color c = CheckCollision(mouseX, mouseY) ? fontColorHover : fontColor;
	buttonFont->DrawText(NULL, buttonText.c_str(), -1, &buttonRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER , D3DCOLOR_ARGB(255, c.GetRed(), c.GetGreen(), c.GetBlue()) );
}

void UI::Button::AddChild(UserInterface* newElement)
{
	return;
	//throw error //TODO:fix this
}

UI::Button::~Button()
{

	buttonFont->Release();
}