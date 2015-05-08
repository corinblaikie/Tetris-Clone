#ifndef TETRIS_GAME
#define TETRIS_GAME

#include "tetromino.h"
#include "time.h"
#include "color.h"

namespace Tetris
{

	class Game
	{
	public:
		enum State{Over, Paused, Running};

		Game();
		~Game();
		void CheckLines();

		void Drop();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		void Rotate();
		int GetCurrentBlockX(int blockindex);
		int GetCurrentBlockY(int blockindex);
		int GetNextBlockX(int blockindex);
		int GetNextBlockY(int blockindex);
		Color GetCurrentColor() const;
		Color GetNextColor() const;
		
		const int GetBoardHeight() const;
		const int GetBoardWidth() const;
		const Color GetBoardPosition(int x, int y) const;
		bool CheckCollision(int offSetX, int offSetY);
		void NewTetromino();
		const int GetSpeed() const;
		const int GetScore() const;
		const int GetLevel() const;
		const int GetTotalLines() const;
		const State GetState() const;
		void SetState(State newState);
		
		bool IsDirty();
		void ClearDirty();

	protected:	
		int speed;
		int score;
		int level;
		int totalLines;
		State currentState;

		static const int boardWidth = 10;
		static const int boardHeight = 18;
		Color board[boardWidth][boardHeight];

		Tetromino* currentTetrominoPtr;
		Tetromino* nextTetrominoPtr;
		bool move(int x, int y);
		void removeLine(int position);
		void updateScore(int lines);
		void updateLevel(int lines);

		// prevent copying
		// TODO: implement deep/lazy copying
		Game(const Game&);
		Game operator=(const Game&);

	};

}

#endif 