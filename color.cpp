#include "color.h"
#include <sstream>

Tetris::Color::Color()
{
	SetEmpty();
}

Tetris::Color::Color(int r, int g, int b)
{
	_red = r;
	_green = g;
	_blue = b;
	_isEmpty = false;
}

const bool Tetris::Color::IsEmpty() const 
{
return _isEmpty;
}

void Tetris::Color::SetEmpty()
{
	_red = 0;
	_green = 0;
	_blue = 0;
	_isEmpty = true;
}

const int Tetris::Color::GetRed() const
{
return _red;
}

const int Tetris::Color::GetGreen() const
{
return _green;
}

const int Tetris::Color::GetBlue() const
{
return _blue;
}


void Tetris::Color::SetRed(int r)
{
				_red = r;
}

void Tetris::Color::SetGreen(int g)
{
				_green = g;
}

void Tetris::Color::SetBlue(int b)
{
				_blue = b;
}


const std::string Tetris::Color::ToHex() const
{
	std::ostringstream hexStringStream;
	const char* hexChars = "0123456789ABCDEF";

	//convert R
	for (int i=2*sizeof(int) - 1; i>=0; i--) {
		hexStringStream << "0123456789ABCDEF"[((this->_red >> i*4) & 0xF)];
	}

	//convert G
	for (int i=2*sizeof(int) - 1; i>=0; i--) {
		hexStringStream << "0123456789ABCDEF"[((this->_green >> i*4) & 0xF)];
	}

	//convert B
	for (int i=2*sizeof(int) - 1; i>=0; i--) {
		hexStringStream << "0123456789ABCDEF"[((this->_blue >> i*4) & 0xF)];
	}
	return hexStringStream.str();


	
}
