#include"CGame.h"
#include"TextureManager.h"
#include"GridManager.h"
#include"CGameAI.h"

//Final game texture/clips
SDL_Texture* gameEndTextures = NULL;
SDL_Rect gameEndClips[3];
SDL_Rect gameEndDimensions;


//grid. Board was a better name
GridManager Grid;

//game state enum, so we have track
//of current game state
GAME_STATE game_state;

//mouse position on screen
int mousex;
int mousey;

//is ia playing
bool isIAactive = true;

//Game constructor
CGame::CGame()
{
	isRunning = true;

	gameRenderer = NULL;

	gameWindow = NULL;
}

//Game destructor
CGame::~CGame()
{
	Close();
}

//Game Initialize
bool CGame::Initialize(const char* title, int width, int height)
{
	//initialize sdl
	if ( (SDL_Init(SDL_INIT_EVERYTHING)) != 0 )
	{
		printf(".::CANT::INITIALIZE::SDL2::- %s -::.\n", SDL_GetError() );
		return false;
	}

	//initialize sdl image
	if ((IMG_Init(IMG_INIT_PNG)) != IMG_INIT_PNG)
	{
		printf(".::CANT::INITIALIZE::SDL_IMAGE::- %s -::.\n", IMG_GetError());
		return false;
	}

	//create game window
	gameWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, 
								SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if (gameWindow == NULL)
	{
		printf(".::CANT::CREATE::WINDOW::- %s -::.\n", SDL_GetError());
		return false;
	}

	//create game renderer
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gameRenderer == NULL)
	{
		printf(".::CANT::CREATE::RENDERER::- %s -::.\n", SDL_GetError());
		return false;
	}

	printf("SPACE: draw grid on console(debug).\nLSHIFT: Restart game.\nESCAPE: Game End.\nLCTRL: active/desactive IA.\n");

	return true;
}

//Game File Management
void CGame::Manage()
{
	//initialize grid
	Grid.Initialize();
	
	//load textures
	Grid.LoadTexture(gameRenderer, "assets/textures/circle.png", "assets/textures/cross.png");

	gameEndTextures = TextureManager::LoadTexture(gameRenderer, SDL_TRUE, "assets/textures/finalstates.png");
	gameEndClips[0] = { 0, 0, 600, 200 }; //player one wins
	gameEndClips[1] = { 0, 200, 600, 200 }; //player two wins
	gameEndClips[2] = { 0, 400, 600, 200 }; //tie

	gameEndDimensions = { 0, 0, 600, 200 };

	//initializes game state
	game_state = GAME_PLAYER1;
}

//Game Loop
void CGame::Loop()
{
	Manage();
	while (isRunning)
	{
		Logic();
		Render();
		CleanUp();
	}
}

//Game logic handler
void CGame::Logic()
{
	bool gridChange = false;
	bool gameRestart = false;

	//Register keyboard and mouse input
	while (SDL_PollEvent(&gameEvent) != 0)
	{
		//If event is a Quit event, then stop running
		if (gameEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}

		//MOUSE INPUT
		if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			if(game_state == GAME_PLAYER1)
			{
				gridChange = Grid.ModifyGrid(GAME_PLAYER1);
			}
			else if(game_state == GAME_PLAYER2 && isIAactive == false)
			{
				//player two plays just if IA is desactivated
				gridChange = Grid.ModifyGrid(GAME_PLAYER2);
			}
			else if (game_state == GAME_TIE || game_state == GAME_WON_P1 || game_state == GAME_WON_P2)
			{
				gameRestart = true;
			}
		}

		//KEYBOARD
		if (gameEvent.type == SDL_KEYDOWN)
		{
			if (gameEvent.key.keysym.sym == SDLK_SPACE)
			{
				Grid.DrawConsole(); //Draw grid on console
			}
			else if (gameEvent.key.keysym.sym == SDLK_LSHIFT)
			{
				gameRestart = true;
				printf(".::GAME::RESTART::.\n");
			}
			else if (gameEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
				printf(".::GAME::END::.\n");
			}
			else if (gameEvent.key.keysym.sym == SDLK_LCTRL)
			{
				isIAactive = !isIAactive;
				if (isIAactive)
				{
					printf("IA Activated.\n");
				}
				else
				{
					printf("IA Desactivated.\n");
				}
			}
		}

	}


	//AI makes it's turn if is playing
	if (game_state == GAME_PLAYER2 && isIAactive == true)
	{
		//rebuild grid
		//here we rebuild the grid stored on Grid object.

		int* gridptr = Grid.ReturnGrid(); //take pointer of the first element of the array

		//copies values from gridptr to new array
		int currentGrid[3][3]; 

		printf("Rebuilding grid...\n");
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				currentGrid[i][j] = *gridptr;
				gridptr++; //dangerous pointer arethmetics!1!11!!
			}
		}

		//now we can pass the current grid normally
		printf("Making new move with current grid...\n"); //mostly debug code...
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("%d ", currentGrid[i][j]);
			}
			printf("\n");
		}

		//Finally, we calculate the next move using minimax. It returns a board position.
		BoardPosition newPos = CGameAI::NextMove(currentGrid);

		//Using the returned position, we pass it to AIModGrid.
		Grid.AIModGrid(newPos);

		//i love debug code as much as garlic bread!1!!1
		printf("New AI move: %d, %d\n", newPos.x, newPos.y);

		//A player (IA) made a change, so gridchange has to turn true.
		gridChange = true;
	}
	
	//verify victory - Tie - Full
	if (Grid.VerifyVictory() == GAME_PLAYER1)
	{
		game_state = GAME_WON_P1;
	}
	else if (Grid.VerifyVictory() == GAME_PLAYER2)
	{
		game_state = GAME_WON_P2;
	}

	//Change player states. If grind changed change player.
	if (gridChange && game_state == GAME_PLAYER1)//IF P1 PLAYING -> P2 PLAYS
	{
		game_state = GAME_PLAYER2;
	}
	else if (gridChange && game_state == GAME_PLAYER2) //IF P2 PLAYS -> P1 PLAYING
	{
		game_state = GAME_PLAYER1;
	}

	//Verifies tie. IF grid is full && nobody won -> tie
	if (Grid.GridFull() && (game_state != GAME_WON_P1 && game_state != GAME_WON_P2))
	{
		game_state = GAME_TIE;
	}
	
	//IF GAME STOPED -> REINITIALIZE GRID && P1 PLAYS
	if (gameRestart) 
	{
		Grid.Initialize();
		game_state = GAME_PLAYER1;
		system("cls");
	}
}

//Game Render
void CGame::Render()
{
	//Clear Renderer
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameRenderer);

	//Draw Graphical grid
	Grid.DrawGrid(gameRenderer);

	//Draw final states
		//if game is tie
	switch (game_state)
	{
	case GAME_WON_P1:
		TextureManager::RenderTexture(gameRenderer, gameEndTextures, &gameEndClips[0], &gameEndDimensions);
		break;
	case GAME_WON_P2:
		TextureManager::RenderTexture(gameRenderer, gameEndTextures, &gameEndClips[1], &gameEndDimensions);
		break;
	case GAME_TIE:
		TextureManager::RenderTexture(gameRenderer, gameEndTextures, &gameEndClips[2], &gameEndDimensions);
		break;
	default:
		break;
	}

	//Updates Renderer
	SDL_RenderPresent(gameRenderer);
}

//File clean up
void CGame::CleanUp()
{
	
}

//Game close
void CGame::Close()
{	
	Grid.CleanTexture();

	//Destroy renderer
	SDL_DestroyRenderer(gameRenderer);

	//Destroy game window
	SDL_DestroyWindow(gameWindow); //implicit window surface destroy

	//Clean textures
	SDL_DestroyTexture(gameEndTextures);
	gameEndTextures = NULL;

	//Close SDL and subsystems
	SDL_Quit();
	IMG_Quit();
}