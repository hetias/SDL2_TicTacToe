#include"CGame.h"
#include"GridManager.h"
#include"TextureManager.h"

GridManager::GridManager()
{
	Cross = NULL;
	Circle = NULL;
}

void GridManager::LoadTexture(SDL_Renderer* renderer, const char* cross_file, const char* circle_file)
{
	if ( (Cross = TextureManager::LoadTexture(renderer, SDL_FALSE,cross_file)) == NULL)
	{
		printf(".::CANT::CROSS::TEXTURE::.");
	}
	if( (Circle = TextureManager::LoadTexture(renderer,SDL_FALSE, circle_file)) == NULL)
	{ 
		printf(".::CANT::CIRCLE::TEXTURE::.");
	}
}

void GridManager::DrawGrid(SDL_Renderer* renderer)
{
	int xpos = 0;
	int ypos = 0;

	//a rectangle on 0,0
	//with height and width of 200
	SDL_Rect markRect = { 0, 0, 200, 200 };


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//we change x and y properties based on i and j,
			//next we multiply it by marks dimensions
			markRect = { j * markDimension, i * markDimension, markDimension, markDimension };

			//depending on what the actual grid position has
			//we draw a circle or a cross
			if (Grid[i][j] == 1)
			{
				SDL_RenderCopy(renderer, Circle, NULL, &markRect);
			}
			else if (Grid[i][j] == 2)
			{
				SDL_RenderCopy(renderer, Cross, NULL, &markRect);
			}
		}
	}
}

void GridManager::DrawConsole()
{
	//just writing matrices on console 
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", Grid[i][j]);
		}
		printf("\n");
	}
}

void GridManager::CleanTexture()
{
	SDL_DestroyTexture(Cross);
	Cross = NULL;
	SDL_DestroyTexture(Circle);
	Circle = NULL;
}

bool GridManager::ModifyGrid(int player)
{
	//get mouse x and w
	int grid_x,grid_y;
	SDL_GetMouseState(&grid_x, &grid_y);
	
	//we dived that by mark dimensions
	//and we get the equivalent i and j
	int i = grid_y /markDimension;
	int j = grid_x / markDimension;

	//now if there is blank space
	//change it
	if (Grid[i][j] == 0)
	{
		Grid[i][j] = player;
		return true;
	}

	//just some debug stuff to see the screen to matrix convertion
	//printf("cuadrant i: %d, cuadrant j: %d\n", i, j);
	return false;
}

void GridManager::Initialize()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Grid[i][j] = 0;
		}
	}
}

bool GridManager::GridFull()
{
	//so we iterate through the whole matrix
	//and check if we have a  is zero.
	//If we have it, means it's not full. Return false.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Grid[i][j] == 0) return false;
		}
	}

	return true;
}

int GridManager::VerifyVictory()
{
	//i feels this code its kinda weird, even if it works
	//It's preferable to just call one function on game loop, soo
	// 
	//if recieved value is different to 0, return it
	int winner = 0;

	if ((winner = GridHorizontalVerif()) != 0) return winner;

	if ((winner = GridVerticalVerif()) != 0) return winner;

	if ((winner = GridDiagonalPrincVerif()) != 0) return winner;

	if ((winner = GridDiagonalSecVerif()) != 0) return winner;

	return 0;
}

int GridManager::GridHorizontalVerif()
{
	//takes number on the first column(which's can be 0-none 1-circle 2-cross)
	//if isn't 0, look for repetition on the whole row
	//if it repeats three times, return the number(so it returns the player who won)

	//if there aren't enough repetitions, try the next row

	//if doesn't find repetition on any row, return 0. Nobody won
	int pivot = 0;
	int matches = 0;

	for (int i = 0; i < 3; i++)
	{
		if (Grid[i][0] != 0)
		{
			pivot = Grid[i][0];
			for (int j = 0; j < 3; j++)
			{
				if (pivot == Grid[i][j]) matches++;
			}

			if (matches >= 3)
			{
				return pivot;
			}
			else
			{
				matches = 0;
			}
		}
		
	}

	return 0;
}

int GridManager::GridVerticalVerif()
{
	//same that horizontal, but applied differently
	int pivot = 0;
	int matches = 0;

	for (int i = 0; i < 3; i++)
	{
		if (Grid[0][i] != 0)
		{
			pivot = Grid[0][i];
			for (int j = 0; j < 3; j++)
			{
				if (pivot == Grid[j][i]) matches++;
			}

			if (matches >= 3)
			{
				return pivot;
			}
			else
			{
				matches = 0;
			}
		}

	}

	return 0;
}

int GridManager::GridDiagonalPrincVerif()
{
	//takes first element on the matrix if it's different from 0
	//now checks through the principal diagonal for the same num on repetition

	//if repeated three times, return the number

	int pivot = 0;
	int matches = 0;

	if (Grid[0][0] != 0) pivot = Grid[0][0];

	for (int i = 0; i < 3; i++)
	{
		if (pivot == Grid[i][i]) matches++;
	}

	if (matches >= 3) return pivot;

	return 0;
}

int GridManager::GridDiagonalSecVerif()
{
	//similiar like GridDiagonalPrincVerif, but slightly changed 
	//for secondary diagonal
	int pivot = 0;
	int matches = 0;

	//chechs for number on x ->
	//- - x
	//- - -
	//- - -
	if (Grid[2][0] != 0) pivot = Grid[2][0];

	for (int i = 0; i < 3; i++)
	{
		if (Grid[i][2 - i] == pivot) matches++;
	}

	if (matches >= 3) return pivot;

	return 0;
}