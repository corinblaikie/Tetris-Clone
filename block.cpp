#include "block.h"

Tetris::Block::Block()
{

}

Tetris::Block::Block(int x, int y)
{
	_positionX = x;
	_positionY = y;
}

const int Tetris::Block::GetPositionX() const
{
	return _positionX;
}

const int Tetris::Block::GetPositionY() const
{
	return _positionY;
}

void Tetris::Block::SetPosition(int x, int y)
{
	_positionX = x;
	_positionY = y;
}

void Tetris::Block::SetPositionX(int x)
{
	_positionX = x;
}

void Tetris::Block::SetPositionY(int y)
{
	_positionY = y;
}