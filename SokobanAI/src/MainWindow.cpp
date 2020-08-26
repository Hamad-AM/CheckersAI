#include "Application.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	Application app(SCREEN_WIDTH, SCREEN_HEIGHT);

	app.run();

	return 0;
}