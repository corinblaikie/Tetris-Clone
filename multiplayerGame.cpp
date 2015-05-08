#include "multiplayerGame.h"
#include "color.h"

MultiplayerGame::MultiplayerGame()
:Game()
{
	linesRecieved = 0;
}

void MultiplayerGame::CheckLines()
{
	while (linesRecieved > 0)
	{
		addLine();
		linesRecieved--;
	}
}


void MultiplayerGame::ReceiveLine()
{
	linesRecieved++;
}

void MultiplayerGame::addLine()
{
	// Get a random number within the board width
	int random = boardWidth / 2; //TODO: get actual random number
	// the new line must have 1 block missing otherwise it would disappear


	// add line to bottom of the board
	//move rows down
	for (int y = boardHeight-1; y > 0; y--)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			board[x][y] = board[x][y-1];
		}
	}

	//add the new line to the bottom row
	for (int x = 0; x < boardWidth; x++)
	{
		if (x == random)
		{
			board[x][0].SetEmpty();
		}
		else
		{
			board[x][0] = Tetris::Color(160, 160, 160);
		}
	}
}
