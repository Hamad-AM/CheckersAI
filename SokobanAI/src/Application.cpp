#include "Application.h"
#include <stdio.h>

//// window to be rendered to
//SDL_Window* window = NULL;
//
//// The surface contained by the window
//SDL_Surface* screenSurface = NULL;
//
//// Initialize SDL
//if (SDL_Init(SDL_INIT_VIDEO) < 0)
//{
//	printf("SDL Could not Initialize! SDL_Error: %s\n", SDL_GetError());
//}
//else
//{
//	// Create window 
//	window = SDL_CreateWindow("Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//	if (window == NULL)
//	{
//		printf("SDL window could not initialize! SDL_Error: %s\n", SDL_GetError());
//	}
//	else
//	{
//		// Get Window surface
//		screenSurface = SDL_GetWindowSurface(window);
//
//		// Fill surface white
//		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
//
//		// Update the surface
//		SDL_UpdateWindowSurface(window);
//
//		// Wait two seconds
//		SDL_Delay(2000);
//	}
//}
//
//// Destroy window
//SDL_DestroyWindow(window);
//
////Quit sdl subsystem
//SDL_Quit();

Application::Application(const int inSCREEN_WIDTH, const int inSCREEN_HEIGHT) : SCREEN_HEIGHT(inSCREEN_HEIGHT), SCREEN_WIDTH(inSCREEN_WIDTH)
{
	Application::init();
}

Application::~Application()
{
}

bool Application::init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Could not Initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create window 
		m_Window = SDL_CreateWindow("Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_Window == NULL)
		{
			printf("SDL window could not initialize! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Get Window surface
			m_ScreenSurface = SDL_GetWindowSurface(m_Window);
		}
	}

	return success;
}

bool Application::loadMedia()
{
	return false;
}

void Application::close()
{
}

