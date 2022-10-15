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

void GridManager::AIModGrid(BoardPosition pos)
{
	Grid[pos.x][pos.y] = 2;
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
	//oldest version of this was kinda absurd

	for (int i = 0; i < 3; i++)
	{
		if (Grid[i][0] != 0) {
			if (Grid[i][0] == Grid[i][1] && Grid[i][2] ==Grid[i][1]) return Grid[i][0];
		}
		
	}
	

	return 0;
}

int GridManager::GridVerticalVerif()
{
	//same that horizontal, but applied differently
	for (int i = 0; i < 3; i++)
	{
		if (Grid[0][i] != 0)
		{
			if (Grid[0][i] == Grid[1][i] && Grid[2][i] == Grid[1][i]) return Grid[0][i];
		}
		
	}

	return 0;
}

int GridManager::GridDiagonalPrincVerif()
{
	if (Grid[0][0] != 0)
	{
		if (Grid[0][0] == Grid[1][1] && Grid[2][2] == Grid[1][1]) return Grid[0][0];
	}
	return 0;
}

int GridManager::GridDiagonalSecVerif()
{
	if (Grid[0][2] != 0)
	{
		if (Grid[0][2] == Grid[1][1] && Grid[2][0] == Grid[1][1])return Grid[0][2];
	}
	return 0;
}

int* GridManager::ReturnGrid()
{
	int* gridptr = &Grid[0][0];

	return gridptr;
}