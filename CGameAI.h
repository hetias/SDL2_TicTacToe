#ifndef CGAMEAI_H
#define CGAMEAI_H

#include"CGame.h"

class CGameAI
{
public:
	//calculate next IA move
	static BoardPosition NextMove(int board[3][3]);

private:
	//minimax function, here's the magic
	static int Minimax(int board[3][3], int depth, bool isMax);
	//static evaluations, so it knows final outcomes
	static int StaticEvaluation(int board[3][3]);
	//i could put this on static evaluation, well i didn't but i'm so tired
	static bool BoardFull(int board[3][3]);

};

#endif // !CGAMEAI_H
