#include<stdio.h>
#include"CGame.h"

int main(int argc, char *argv[])
{
	//Creates game object
	CGame Game;
	
	//Initialize game
	if ( !(Game.Initialize("Game", 600, 600)))
	{
		printf(".::GAME::COULDNT::INITIALIZE::.");
		return -1;
	}

	//Start game loop
	Game.Loop(); //1.Logic 2.Render 3.CleanUp

	return 0;
}