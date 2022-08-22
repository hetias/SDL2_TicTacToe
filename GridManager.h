#ifndef GRIDMANAGER_H
	#define GRIDMANAGER_h

#include"CGame.h"

class GridManager
{
public:
	//grid constructor
	GridManager();
	//initializes grid, all to 0
	void Initialize();
	//recives current player, return false
	//if couldn't modify grid(not really needed?)
	bool ModifyGrid(int player);
	//load grid/boar textures
	void LoadTexture(SDL_Renderer* renderer, const char* cross_file, const char* circle_file);
	//free textures
	void CleanTexture();
	//draw grid on screen
	void DrawGrid(SDL_Renderer* renderer);
	//draws grid on console, for debbuging
	void DrawConsole();
	
	//chech if grid is full
	bool GridFull();

	//chech if a common pattern exist
	//on horizontal-vertical-diagonals
	//return: 1- player one 2-player two 0-nobody won
	int GridHorizontalVerif();
	int GridVerticalVerif();
	int GridDiagonalPrincVerif();
	int GridDiagonalSecVerif();
	
	//call all grid verification functions
	//returns 0 just if nobody won
	int VerifyVictory();

private:
	//actual grid/board
	int Grid[3][3];
	//size of crosses/circles on screen
	int markDimension = 200;
	SDL_Texture* Cross;
	SDL_Texture* Circle;
};



#endif