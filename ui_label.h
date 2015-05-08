#include "UserInterface.h"
#include <string>
#include <d3d9.h> 
#include <d3dx9.h>
#include "color.h"

namespace UI
{
	using std::string;

	class Label: public UserInterface
	{
	public:
		Label(std::string labelText, long positionX, long positionY, long width, long height, Tetris::Color fontColor, LPDIRECT3DDEVICE9 directXDevice);
		Label::~Label();
		void Label::Draw(LPDIRECT3DDEVICE9 directXDevice, int x, int y);
		bool Label::CheckCollision(int x, int y);
		void Label::MouseClick(int x, int y, HWND hwnd);
		virtual void Label::AddChild(UserInterface* newElement);
	private:
		Label(const Label&);
		Label operator=(const Label&);

		std::string labelText;
		Tetris::Color fontColor;
		RECT labelRect;
		LPD3DXFONT labelFont;
		UINT msg;
	};
}