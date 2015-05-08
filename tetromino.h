#ifndef TETROMINO
#define TETROMINO

#include "point2d.h"
#include "color.h"

namespace Tetris
{

class Tetromino
{
public:	
	static Tetromino * RandomTetromino(int offsetX, int offsetY);
	static const int BlockCount = 4;
	void Move(int x, int y);
	void RotateLeft();
	void RotateRight();	
	const int GetBlockPositionX(int blockIndex) const;
	const int GetBlockPositionY(int blockIndex) const;	
	const Color GetColor() const;	
	const bool IsDirty() const;
	void ClearDirty();

protected:
	Tetromino(int offsetX = 0, int offsetY = 0);
	void setColor(int r, int g, int b);	
	Point2D<int> blocks[BlockCount];
	int offsetX;
	int offsetY;
	bool dirty;
	Color color;
	static const int martixSize = 3;	
};

// I, J, L, O, S, T, Z Tetrominos.
// As the only things that are overwritten in these inherited classes are the constructors
// they could be implemented simply as Tetromino objects, and the constructors 
// combined into one constructor with case statements. 
// However this would restrict the ability to explicitly construct different shapes
// without the use of an variable (enum) for shape type.

class TetrominoI: public Tetromino
{
public:
	TetrominoI(int offsetX, int offsetY);
private:
	static const int martixSize = 4;
};

class TetrominoJ: public Tetromino
{
public:
	TetrominoJ(int offsetX, int offsetY);
};

class TetrominoL: public Tetromino
{
public:
	TetrominoL(int offsetX, int offsetY);
};

class TetrominoO: public Tetromino
{
public:
	TetrominoO(int offsetX, int offsetY);
private:
	static const int martixSize = 4;
};

class TetrominoS: public Tetromino
{
public:
	TetrominoS(int offsetX, int offsetY);
private:
	static const int martixSize = 4;
};

class TetrominoT: public Tetromino
{
public:
	TetrominoT(int offsetX, int offsetY);
};

class TetrominoZ: public Tetromino
{
public:
	TetrominoZ(int offsetX, int offsetY);
private:
	static const int martixSize = 4;
};


class RangeException : public std::runtime_error {
 public: 
	 RangeException() : std::runtime_error("Range Exception"){ }
 };


}

#endif