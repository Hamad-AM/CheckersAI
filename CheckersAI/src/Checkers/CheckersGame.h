#pragma once

#include <memory>
#include <vector>

#include "Square.h"
#include "Player.h"
#include "ManualPlayer.h"
#include "AIPlayer.h"
#include "Board.h"

//using Board = std::vector<std::vector<Square>>;

class CheckersGame
{
public:
	CheckersGame(Color manualPlayersColor);

	void play();
private:
	void initialize();
	void checkIfGameOver(bool& gameOver);
	std::vector<Move> getValidMoves();

	Color turn;
	Color winningColor;

	Board board;

	std::unique_ptr<Player> whitePlayer;
	std::unique_ptr<Player> blackPlayer;
};

