#include "game.h"

//------------ public methods -----------------

bool Tetris::Game::IsDirty()
{
	return currentTetrominoPtr->IsDirty();
}

Tetris::Game::Game()
: score(0), level(0), totalLines(0) ,speed(300), currentState(Running)
{ 
	//ensure array is set to empty color objects 
	//(the default constructor is to an empty color 
	// so this isn't required but is just safe keeping 
	//incase the color class gets changed
	for(int y = 0; y < boardHeight ; y++)
	{
		for(int x = 0; x < boardWidth; x++)
		{
			board[x][y].SetEmpty();
		}
	}

	srand((unsigned)time(0)); //seed random numbers
	currentTetrominoPtr = Tetromino::RandomTetromino((boardWidth/2)-2, boardHeight - 1);
	nextTetrominoPtr = Tetromino::RandomTetromino((boardWidth/2)-2, boardHeight - 1);
}

Tetris::Game::~Game()
{
	delete currentTetrominoPtr;
	delete nextTetrominoPtr;
}

void Tetris::Game::NewTetromino()
{
	// Put current tetromino into the board
	for(int i = 0; i < currentTetrominoPtr->BlockCount; i++)
	{
		int x = currentTetrominoPtr->GetBlockPositionX(i);
		int y = currentTetrominoPtr->GetBlockPositionY(i);
		if (!((x < 0) || (x >= boardWidth) || (y < 0) || (y >= boardHeight) || (!board[x][y].IsEmpty())) )
		{
			board[x][y] = currentTetrominoPtr->GetColor();
		}	
	}
	delete currentTetrominoPtr;
	currentTetrominoPtr = nextTetrominoPtr;
	nextTetrominoPtr = Tetromino::RandomTetromino(boardWidth/2-2, boardHeight - 1); //TODO remove const numbers
	CheckLines();
	if (CheckCollision(0, 0))
	{
		currentState = Over;
	}
}

void Tetris::Game::CheckLines()
{
	int lines = 0;
	bool lineFound = false;
	for(int y = 0; y < boardHeight; y++)
	{
		//TODO: could use a tidyup
		lineFound = true;
		for(int x = 0; x < boardWidth; x++)
		{
			if (board[x][y].IsEmpty())
			{
				lineFound = false;
			}
		}
		if(lineFound)
		{
			lines++;
			removeLine(y);
			y--;
		}
	}

	
	this->updateScore(lines);
	this->updateLevel(lines);
}

bool Tetris::Game::move(int x, int y)
{
	if (!CheckCollision(x, y))
	{
		 currentTetrominoPtr->Move(x, y);
		 return true;
	}
	return false;
}

void Tetris::Game::MoveDown(){ move(0, - 1); }

void Tetris::Game::MoveLeft(){ move(-1, 0); }

void Tetris::Game::MoveRight(){ move(1, 0); }

void Tetris::Game::Rotate()
{
	currentTetrominoPtr->RotateLeft();
	if (CheckCollision(0, 0))
	{
		currentTetrominoPtr->RotateRight();
	}			
}
void Tetris::Game::Drop()
{ 
	while (move(0, -1)){}	
	NewTetromino();
}

int Tetris::Game::GetCurrentBlockX(int blockindex){ return currentTetrominoPtr->GetBlockPositionX(blockindex); }
int Tetris::Game::GetCurrentBlockY(int blockindex){ return currentTetrominoPtr->GetBlockPositionY(blockindex); }
int Tetris::Game::GetNextBlockX(int blockindex){ return nextTetrominoPtr->GetBlockPositionX(blockindex); }
int Tetris::Game::GetNextBlockY(int blockindex){ return nextTetrominoPtr->GetBlockPositionY(blockindex); }
Tetris::Color Tetris::Game::GetCurrentColor() const { return currentTetrominoPtr->GetColor(); }
Tetris::Color Tetris::Game::GetNextColor() const { return nextTetrominoPtr->GetColor(); }


void Tetris::Game::ClearDirty()
{
	currentTetrominoPtr->ClearDirty();
}

const int Tetris::Game::GetBoardHeight() const{ return boardHeight;}
const int Tetris::Game::GetBoardWidth() const{ return boardWidth;}
const int Tetris::Game::GetSpeed() const{ return speed;} 
const int Tetris::Game::GetTotalLines() const{ return totalLines;}



bool Tetris::Game::CheckCollision(int offSetX, int offSetY)
{
	for(int i = 0; i < currentTetrominoPtr->BlockCount; i++)
	{
		int x = currentTetrominoPtr->GetBlockPositionX(i) + offSetX;
		int y = currentTetrominoPtr->GetBlockPositionY(i) + offSetY;
		if ((x < 0) || (x >= boardWidth) || (y < 0) || (y >= boardHeight) || (!board[x][y].IsEmpty()) )
		{
			return true;
		}
	}


	return false;
}

const Tetris::Color Tetris::Game::GetBoardPosition(int x, int y) const
{
	if ((x < 0) || (x >= boardWidth) || (y < 0) || (y >= boardHeight))
		return Tetris::Color();
	else
		return board[x][y];
}

// private methods

void Tetris::Game::removeLine(int position)
{
	//move rows down
	for(int y = position; y < boardHeight - 1; y++)
	{
		for(int x = 0; x < boardWidth; x++)
		{
			board[x][y] = board[x][y+1];
		}
	}

	//clear top row
	for(int x = 0; x < boardWidth; x++)
	{
		board[x][boardHeight-1].SetEmpty();
	}
}

void Tetris::Game::updateScore(int lines)
{
	
	/*
	Single = (level+1)*40 one line is cleared.</li>
	Double = (level+1)*100 two lines are simultaneously cleared.
	Triple = (level+1)*300 three lines are simultaneously cleared.
	Tetris = (level+1)*1200 four lines are simultaneously cleared.
	*/
	switch (lines)
	{
	case 1:
		score += (level+1)*40;
		break;
	case 2:
		score += (level+1)*100;
		break;
	case 3:
		score += (level+1)*300;
		break;
	case 4:
		score += (level+1)*1200;
		break;

	}
}

void Tetris::Game::updateLevel(int lines)
{
	while (lines > 0)
	{
		totalLines += 1;
		if ((totalLines % 10) == 0)
		{
			level += 1;
		
			if (speed > 20)
			{
				speed -= 20;							    
			}
		}
		lines --;
	}
}

const int Tetris::Game::GetScore() const 
{ 
	return score; 
}

const int Tetris::Game::GetLevel() const 
{ 
	return level; 
}

const Tetris::Game::State Tetris::Game::GetState() const 
{ 
	return currentState; 
}

void Tetris::Game::SetState(State newState)
{
	currentState = newState;
}