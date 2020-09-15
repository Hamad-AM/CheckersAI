#include "GameWindow.h"
#include "Checkers/CheckersGame.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	//GameWindow app(SCREEN_WIDTH, SCREEN_HEIGHT);

	CheckersGame game(WHITE);

	game.play();

	return 0;
}