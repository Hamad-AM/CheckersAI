#pragma once

#include "Player.h"

#include "Board.h"

class ManualPlayer : public Player
{
public:
	ManualPlayer(Color color);

	Move chooseMove(Board& board, std::vector<Move> validMoves) override;
};

