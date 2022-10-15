#include"CGameAI.h"
#include<algorithm>

//maximazer ad = 10
//minimazer ad = -10

//workin minimax!!1!!1!
//if you play the pattern 4, 6, 3 the IA it's gonna make the wrong choise.
//it may just be a limitation of the algorithm? maybe, i don't think so.
//maybe there's something i can change about it, but i dont figure it out.


BoardPosition CGameAI::NextMove(int board[3][3])
{
	int bestEval = -99;
	BoardPosition bestPos = { -1, -1 };

	//traverse board
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//add mark on new position if its free
			if (board[i][j] == 0)
			{
				printf("position %d,%d free...\n", i,j);

				board[i][j] = 2;
				
				int score = Minimax(board, 0, false);
				
				printf("Score obtained: %d...\n", score);

				board[i][j] = 0;
				
				if (score > bestEval)
				{
					bestEval = score;
					bestPos.x = i;
					bestPos.y = j;
					printf("New best evaluation: %d position: %d, %d...\n", bestEval, bestPos.x, bestPos.y);
				}
				printf("------------------------\n");
			}
		}
	}

	return bestPos;
}

int CGameAI::Minimax(int board[3][3], int depth, bool isMax)
{
	int score = StaticEvaluation(board);

	//base case
	if (score != 0) return score;

	if (BoardFull(board)) return 0;

	//recursive case
	if (isMax)
	{
		//maximazer turn (the player)
		int best = -90;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				//add mark
				if (board[i][j] == 0) //if position is available
				{
					board[i][j] = 1;
					
					int score = Minimax(board, depth+1, false);
					
					board[i][j] = 0;
					
					best = std::max(best, score);
				}
				
			}
		}

		return (best + depth);
	}
	else
	{
		//minimizer turn (the baddy)
		int best = 90;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				//add mark
				if (board[i][j] == 0)
				{
					board[i][j] = 2;
					
					int score = Minimax(board, depth+1, true);
					
					board[i][j] = 0;
					
					best = std::min(best, score);
				}
			}
		}
		return (best - depth);
	}
}

int CGameAI::StaticEvaluation(int board[3][3])
{
	//horizontal
	for (int i = 0; i < 3; i++)
	{
		if (board[i][0] != 0)
		{
			if (board[i][0] == board[i][1] && board[i][2] == board[i][1])
			{
				if (board[i][0] == 1)
				{
					return 10;
				}
				else
				{
					return -10;
				}
			}
		}
		
	}

	//vertical
	for (int i = 0; i < 3; i++)
	{
		if (board[0][i] != 0)
		{
			if (board[0][i] == board[1][i] && board[2][i] == board[1][i])
			{
				if (board[0][i] == 1)
				{
					return 10;
				}
				else
				{
					return -10;
				}
			}
		}
		
	}

	//first diagonal
	if (board[0][0] != 0)
	{
		if (board[0][0] == board[1][1] && board[2][2] == board[1][1])
		{
			if (board[0][0] == 1)
			{
				return 10;
			}
			else
			{
				return -10;
			}
		}
	}
	
	//second diagonal
	if (board[0][2] != 0)
	{
		if (board[0][2] == board[1][1] && board[2][0] == board[1][1])
		{
			if (board[0][2] == 1)
			{
				return 10;
			}
			else
			{
				return -10;
			}
		}
	}

	return 0;
}

bool CGameAI::BoardFull(int board[3][3])
{

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != 0) 
			{
				return false;
			}
		}
	}

	return true;
}