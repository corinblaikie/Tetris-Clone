#include "ui_panel.h"
#include "resource.h"
#include <algorithm>


UI::Panel::Panel(float width, float height, Point2D<float> centerPoint2D, LPDIRECT3DDEVICE9 directXDevice)
{
	if (FAILED(D3DXCreateSprite(directXDevice, &sprite))) return;
	//if (FAILED(D3DXCreateTextureFromFile(directXDevice, "texture\\menu.png", &texture))) return;

	//if (FAILED(D3DXCreateTextureFromResource(directXDevice, "texture\\menu.png", &texture))) return;
	if (FAILED(D3DXCreateTextureFromResource(directXDevice, NULL, MAKEINTRESOURCE(IDB_PNG1), &texture)))
	{
		return;
		//TODO: throw error
	}

	alpha = 0;

	position = D3DXVECTOR3(centerPoint2D.x, centerPoint2D.y, 0);

	//scale to screen size
	D3DSURFACE_DESC textureSurfaceDesc;
	texture->GetLevelDesc(0, &textureSurfaceDesc);
   	//float scale = ;
	D3DXVECTOR2 scalingCenter = D3DXVECTOR2(position.x, position.y);
	D3DXVECTOR2 scaling((width / textureSurfaceDesc.Width), (height / textureSurfaceDesc.Height));
	D3DXMATRIX transformationMatrix;
	D3DXMatrixTransformation2D(&transformationMatrix, &scalingCenter, 0.0, &scaling, NULL, 0, NULL);
	sprite->SetTransform(&transformationMatrix);

	texture->GetLevelDesc(0, &textureSurfaceDesc);
	center = D3DXVECTOR3((float)textureSurfaceDesc.Width/2, (float)textureSurfaceDesc.Height/2, 0);

}

void UI::Panel::AddChild(UserInterface* newElement)
{
	children.push_back(newElement);
}

bool UI::Panel::CheckCollision(int x, int y)
{
	for (std::vector<UserInterface*>::iterator it = children.begin(); it != children.end(); ++it) {
		if (((UserInterface*)(*it))->CheckCollision(x, y))
		{
			return true;
		}
	}
	return false;
}

void UI::Panel::MouseClick(int x, int y, HWND hwnd)
{
	for (std::vector<UserInterface*>::iterator it = children.begin(); it != children.end(); ++it) {
		((UserInterface*)(*it))->MouseClick(x, y, hwnd);
	}
}

void UI::Panel::Draw(LPDIRECT3DDEVICE9 directXDevice, int mouseX, int mouseY)
{
	//fade in
	const int fadeSpeed = 10;
	alpha = (alpha < 255 - fadeSpeed) ? alpha + fadeSpeed : 255;
	
	//draw background sprite
	sprite->Begin(D3DXSPRITE_ALPHABLEND);		
	sprite->Draw(texture, NULL, &center, &position, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	sprite->End();

	//draw buttons
	for (std::vector<UserInterface*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		((UserInterface*)(*it))->Draw(directXDevice, mouseX, mouseY);
	}
}

bool UI::Panel::IsDirty(int posX, int posY)
{
	return true;
	//if (alpha < 255)
	//{
	//	return true;
	//}
	//else
	//{
	//	return dirty || CheckCollision(posX, posY);
	//}
}

UI::Panel::~Panel()
{
	sprite->Release();
	texture->Release();

	for (std::vector<UserInterface*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		delete (*it);
	}

	children.clear();

}
