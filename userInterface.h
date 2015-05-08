#ifndef userInterface
#define userInterface

#include <windows.h>
#include <d3d9.h> 
#include <d3dx9.h>
#include <vector>
#include <stdexcept>

namespace UI
{
	class UserInterface
	{
	public:
		virtual void UserInterface::AddChild(UserInterface* newElement) = 0;
		virtual void UserInterface::Draw(LPDIRECT3DDEVICE9 directXDevice, int mouseX, int mouseY) = 0;
		virtual bool UserInterface::CheckCollision(int x, int y) = 0;
		virtual void UserInterface::MouseClick(int x, int y, HWND hwnd) = 0;
		virtual UserInterface::~UserInterface(){};
		unsigned int UserInterface::Count() const{ return children.size(); };
		//bool IsDirty() = 0;
	protected:
		std::vector<UserInterface*> children;
	};


	class InitException : std::runtime_error
	{
		public:
			InitException(std::string Info = "") : std::runtime_error("Initalisation Exception"), extraInfo(Info){}
			std::string GetExtraInfo(){ return extraInfo; }
		private:
			std::string extraInfo;
	};
}

#endif
