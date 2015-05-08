#include <windows.h>
#include <d3d9.h> 
#include <d3dx9.h>
#include <vector>

#include "UserInterface.h"
#include "point2d.h"

namespace UI
{

	class Panel: public UserInterface
	{
	public:
		Panel::Panel(float width, float height, Point2D<float> centerPoint2D, LPDIRECT3DDEVICE9 directXDevice);
		Panel::~Panel();
		bool Panel::CheckCollision(int x, int y);
		void Panel::MouseClick(int x, int y, HWND hwnd);	
		void Panel::AddChild(UserInterface* newElement);
		void Panel::Draw(LPDIRECT3DDEVICE9 directXDevice, int x, int y);
		bool IsDirty(int posX = 0, int posY = 0);
	private:
		Panel(const Panel&);
		Panel operator=(const Panel&);

		bool dirty;
		int alpha;
		LPD3DXSPRITE sprite;
		LPDIRECT3DTEXTURE9 texture;	
		D3DXVECTOR3 position;
		D3DXVECTOR3 center;
	};
}