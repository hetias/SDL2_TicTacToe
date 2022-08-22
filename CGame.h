#ifndef CGAME_H
	#define CGAME_H

#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>

class CGame
{
public:
	//Game constructor
	CGame();

	//Game destructor
	~CGame();

	//Game Initialize
	//initialize sdl2, and other libraries if needed.
	bool Initialize(const char* title, int width, int height);

	//Game File Management
	//loading and unloading of files at runtime
	//is supposed to go here before game loop
	void Manage();

	//Game Main Loop
	void Loop();

	//Game logic handler
	void Logic();

	//Game Render
	void Render();

	//File clean up
	//here memory should be freed on runtime.  if needed
	void CleanUp();

	//Game close
	//close game and sdl things
	void Close();

private:

	SDL_Renderer* gameRenderer;
	SDL_Window* gameWindow;

	bool isRunning;
	SDL_Event gameEvent;
};

#endif // !CGAME_H
