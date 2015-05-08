#include "UserInterface.h"
#include <string>
#include <d3d9.h> 
#include <d3dx9.h>
#include "color.h"

namespace UI
{

	using std::string;

	class Button: public UserInterface
	{
	public:
		Button(std::string buttonText, long positionX, long positionY, long width, long height, Tetris::Color c, UINT callbackMessage, LPDIRECT3DDEVICE9 directXDevice);
		Button::~Button();
		void Button::Draw(LPDIRECT3DDEVICE9 directXDevice, int x, int y);
		bool Button::CheckCollision(int x, int y);
		void Button::MouseClick(int x, int y, HWND hwnd);
		virtual void Button::AddChild(UserInterface* newElement);
	private:
		Button(const Button&);
		Button operator=(const Button&);

		std::string buttonText;
		Tetris::Color fontColor;
		Tetris::Color fontColorHover;
		RECT buttonRect;
		LPD3DXFONT buttonFont;
		UINT msg;
	};

}