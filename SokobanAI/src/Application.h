#pragma once
#include <SDL.h>

class Application
{
public:
	Application(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
	~Application();

	void run();
private:
	bool init();
	bool loadMedia();
	void close();
private:
	SDL_Window* m_Window = NULL;
	SDL_Surface* m_ScreenSurface = NULL;

	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
};

