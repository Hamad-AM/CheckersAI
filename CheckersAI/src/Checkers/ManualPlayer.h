#pragma once

#include "Player.h"

class ManualPlayer : public Player
{
public:
	ManualPlayer(Color color);

	Move chooseMove(std::vector<std::vector<Square>>& board, std::vector<Move> validMoves) override;
};

