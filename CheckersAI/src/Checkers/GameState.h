#pragma once

#include <vector>

#include "Board.h"
#include "Move.h"

struct GameState
{
	Board board;
	int label;
	Move move;
	std::vector<GameState> children;

	GameState(Board boardIn, int labelIn, Move moveIn)
		: board(boardIn), label(labelIn), move(moveIn)
	{
	}
};

