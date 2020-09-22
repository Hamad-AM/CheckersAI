#include "ManualPlayer.h"

#include <iostream>

#include "Move.h"
#include "Square.h"
#include "Board.h"

ManualPlayer::ManualPlayer(Color colorIn)
	: Player(colorIn)
{
}

Move ManualPlayer::chooseMove(Board board, std::vector<Move> validMoves)
{
	int moveIndex;

	std::string printColor = (color == WHITE) ? "white" : "black";
	std::cout << "Choose next move for " << printColor << " ->" << std::endl;
	for (int i = 0; i < validMoves.size(); i++)
	{
		std::cout << "[" << i << "]";
		validMoves[i].write();
		std::cout << ", ";
	}
	std::cout << "Choose Move Index : ";
	std::cin >> moveIndex;

	return validMoves[moveIndex];
}
