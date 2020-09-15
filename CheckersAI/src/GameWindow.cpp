//#include "GameWindow.h"
//#include <stdio.h>
//
//GameWindow::GameWindow(const int inSCREEN_WIDTH, const int inSCREEN_HEIGHT) : SCREEN_HEIGHT(inSCREEN_HEIGHT), SCREEN_WIDTH(inSCREEN_WIDTH)
//{
//	run();
//}
//
//GameWindow::~GameWindow()
//{
//	// Deallocate surface
//	SDL_FreeSurface(m_HelloWorldImg);
//	m_HelloWorldImg = NULL;
//
//	// Destroy Window
//	SDL_DestroyWindow(m_Window);
//	m_Window = NULL;
//
//	SDL_Quit();
//}
//
//bool GameWindow::init()
//{
//	bool success = true;
//
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//	{
//		printf("SDL Could not Initialize! SDL_Error: %s\n", SDL_GetError());
//		success = false;
//	}
//	else
//	{
//		// Create window 
//		m_Window = SDL_CreateWindow("Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//		if (m_Window == NULL)
//		{
//			printf("SDL window could not initialize! SDL_Error: %s\n", SDL_GetError());
//			success = false;
//		}
//		else
//		{
//			// Get Window surface
//			m_ScreenSurface = SDL_GetWindowSurface(m_Window);
//		}
//	}
//
//	return success;
//}
//
//bool GameWindow::loadMedia()
//{
//	bool success = true;
//
//	//Load image
//	m_HelloWorldImg = SDL_LoadBMP("Resources/hello_world.bmp");
//	if (m_HelloWorldImg == NULL)
//	{
//		printf("Unable to load image %s! SDL_Error: %s\n", "Resources/hello_world.bmp", SDL_GetError());
//		success = false;
//	}
//
//	return success;
//}
//
//
//void GameWindow::run()
//{
//	if (!init())
//	{
//		printf("Failed init!");
//	}
//	else
//	{
//		if (!loadMedia())
//		{
//			printf("Failed to Load Media");
//		}
//		else
//		{
//			game();
//			game.start();
//			bool quit = false;
//			SDL_Event e;
//			while (!quit)
//			{
//				while (SDL_PollEvent(&e) != 0)
//				{
//					if (e.type == SDL_QUIT)
//					{
//						quit = true;
//					}
//					else if (e.type == SDL_KEYDOWN)
//					{
//						switch (e.key.keysym.sym)
//						{
//
//						}
//					}
//					game.update();
//				}
//
//				// Apply blit image to surface
//				SDL_BlitSurface(m_HelloWorldImg, NULL, m_ScreenSurface, NULL);
//				// Update the surface
//				SDL_UpdateWindowSurface(m_Window);
//			}
//		}
//	}
//}

