// TODO: refactor rotateRight & RotateLeft and remove duplicate code.
// TODO: throw overflow exceptions for GetBlockPositionX / GetBlockPositionY
// TODO: throw overflow exception in default case in RandomTetromino

#include "tetromino.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Tetris::Tetromino::Tetromino(int x, int y)
{
	offsetX = x;
	offsetY = y;
	dirty = true;
}


Tetris::Tetromino * Tetris::Tetromino::RandomTetromino(int x, int y)
{
	int randomNum = 7;
	while(randomNum > 6)
	{
		randomNum = rand() % 10;
	}

	switch (randomNum)
	{
	case 0:
		  return new TetrominoI(x, y);
	case 1:
		  return new TetrominoJ(x, y);
	case 2:
		  return new TetrominoL(x, y);
	case 3:
		  return new TetrominoO(x, y);
	case 4:
		  return new TetrominoT(x, y);
	case 5:
		  return new TetrominoS(x, y);
	case 6:
		  return new TetrominoZ(x, y);
	default:
		throw RangeException();
	}
}

const Tetris::Color Tetris::Tetromino::GetColor() const
{
	return color;
}


const int Tetris::Tetromino::GetBlockPositionX(int i) const
{
	if (i >= 0 && i < BlockCount)
	{
		return offsetX + blocks[i].x;
	}
	else
	{
		throw RangeException();
	}
	//return 0; 
}

const int Tetris::Tetromino::GetBlockPositionY(int i) const
{
	if (i >= 0 && i < BlockCount)
	{
		return offsetY - blocks[i].y;
	}
	else
	{
		throw RangeException();
	}
	//return 0;
}

void Tetris::Tetromino::Move(int x, int y)
{
	offsetY += y;
	offsetX += x;
	dirty = true;
}


void Tetris::Tetromino::setColor(int r, int g, int b)
{
	color = Color(r,g,b);
}

void Tetris::Tetromino::RotateLeft()
{
	int tmp;
	for(int i = 0; i < BlockCount; i++)
	{
		tmp = blocks[i].x;
		blocks[i].x = martixSize - blocks[i].y;
		blocks[i].y = tmp;
	}
	dirty = true;

}


void Tetris::Tetromino::RotateRight()
{
	int tmp;
	for(int i = 0; i < BlockCount; i++)
	{
		tmp = blocks[i].y;
		blocks[i].y = martixSize - blocks[i].x;
		blocks[i].x = tmp;
	}
	dirty = true;

}

const bool Tetris::Tetromino::IsDirty() const
{
	return dirty;
}
void Tetris::Tetromino::ClearDirty()
{
	dirty = false;
}

//---------------------------------------------
//		Other shapes (I, J, L, O, S, T, Z)
//---------------------------------------------

Tetris::TetrominoI::TetrominoI(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(2, 0);
	blocks[1].SetPosition(2, 1);
	blocks[2].SetPosition(2, 2);
	blocks[3].SetPosition(2, 3);
	setColor(0, 255, 255);
}


Tetris::TetrominoJ::TetrominoJ(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 2);
	blocks[1].SetPosition(2, 2);
	blocks[2].SetPosition(2, 1);
	blocks[3].SetPosition(2, 0);
	setColor(0, 0, 255);
}


Tetris::TetrominoL::TetrominoL(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 0);
	blocks[1].SetPosition(1, 1);
	blocks[2].SetPosition(1, 2);
	blocks[3].SetPosition(2, 2);
	setColor(255, 180, 0);

}

Tetris::TetrominoO::TetrominoO(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 1);
	blocks[1].SetPosition(2, 1);
	blocks[2].SetPosition(1, 2);
	blocks[3].SetPosition(2, 2);
	setColor(255, 255, 0);
}

Tetris::TetrominoS::TetrominoS(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 2);
	blocks[1].SetPosition(2, 2);
	blocks[2].SetPosition(2, 1);
	blocks[3].SetPosition(3, 1);
	setColor(0, 255, 0);
}

Tetris::TetrominoT::TetrominoT(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 1);
	blocks[1].SetPosition(2, 0);
	blocks[2].SetPosition(2, 1);
	blocks[3].SetPosition(2, 2);
	setColor(180, 0, 255);
}

Tetris::TetrominoZ::TetrominoZ(int x, int y) : Tetromino(x, y)
{
	blocks[0].SetPosition(1, 1);
	blocks[1].SetPosition(2, 1);
	blocks[2].SetPosition(2, 2);
	blocks[3].SetPosition(3, 2);
	setColor(255, 0, 0);	
}
