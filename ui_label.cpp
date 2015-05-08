#include "ui_label.h"


UI::Label::Label(string text, long positionX, long positionY, long width, long height,  Tetris::Color c, LPDIRECT3DDEVICE9 directXDevice)
{
	// Create a rectangle to indicate where on the screen it should be drawn	
	labelRect.left = positionX;
	labelRect.right = positionX + width;
	labelRect.top = positionY;
	labelRect.bottom = positionY + height;
	labelText = text;
	fontColor = c;
	labelFont = NULL;
	D3DXCreateFont( directXDevice, height, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &labelFont );
}

bool UI::Label::CheckCollision(int x, int y)
{
	return ((y < labelRect.bottom) && (y > labelRect.top) && (x > labelRect.left) && (x < labelRect.right));
}

void UI::Label::MouseClick(int x, int y, HWND hwnd){}


void UI::Label::Draw(LPDIRECT3DDEVICE9 directXDevice, int mouseX, int mouseY)
{
	labelFont->DrawText(NULL, labelText.c_str(), -1, &labelRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER , D3DCOLOR_ARGB(255, fontColor.GetRed(), fontColor.GetGreen(), fontColor.GetBlue()) );
}

void UI::Label::AddChild(UserInterface* newElement)
{
	return;
	//throw error //TODO:fix this
}

UI::Label::~Label()
{
	labelFont->Release();
}