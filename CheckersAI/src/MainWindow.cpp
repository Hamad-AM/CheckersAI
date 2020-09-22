#include "GameWindow.h"
#include "Checkers/CheckersGame.h"

#include "Checkers/AIPlayer.h"
#include "Checkers/Board.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	//GameWindow app(SCREEN_WIDTH, SCREEN_HEIGHT);

	CheckersGame game(BLACK);

	game.play();

	//AIPlayer player(BLACK);
	//ManualPlayer player2(WHITE);

	//Board board;
	//board.initialize(player, player2);

	//player.chooseMove(board, {});


	return 0;
}