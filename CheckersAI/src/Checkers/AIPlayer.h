#pragma once

#include "Player.h"
#include "Board.h"
#include "Move.h"
#include "GameState.h"

class AIPlayer : public Player
{
public:
	AIPlayer(Color colorIn);

	Move chooseMove(Board board, std::vector<Move> validMoves) override;
private:
	void createNodes(GameState& node, int currentDepth, bool isMaxPlayer);
	GameState minmax(GameState& node, bool isMaxPlayer, int depth);
	
	bool isTerminalState(GameState node);
	int evaluateState(Board& board);

	std::vector<Move> getValidMovesFor(Board& board, Color colorsmove);

	void printNodes(GameState& rootNode) const;

	const int depth = 6;
};

