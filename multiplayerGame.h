#include "color.h"
#include "game.h"

//2 player game
class MultiplayerGame : public Tetris::Game
	{
	public:
		MultiplayerGame();
		void CheckLines();
		void ReceiveLine();

	private:
		void addLine();	

		int linesRecieved;
		
	};



