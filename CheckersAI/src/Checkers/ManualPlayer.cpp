#include "ManualPlayer.h"

#include <iostream>

#include "Move.h"
#include "Square.h"

ManualPlayer::ManualPlayer(Color colorIn)
	: Player(colorIn)
{
}

Move ManualPlayer::chooseMove(std::vector<std::vector<Square>>& board, std::vector<Move> validMoves)
{
	for (Move move : validMoves)
	{
		move.write();
		std::cout << ", " ;
	}

	std::cout << std::endl;

	int startCol;
	int startRow;

	int endCol;
	int endRow;

	std::string printColor = (color == WHITE) ? "white" : "black";
	std::cout << "Choose next move for " << printColor << " ->" << std::endl;
	std::cout << "Start position" << std::endl;
	std::cout << "Column : ";
	std::cin >> startCol;
	std::cout << std::endl;
	std::cout << "Row : ";
	std::cin >> startRow;

	std::cout << std::endl;
	std::cout << "End position" << std::endl;
	std::cout << "Column : ";
	std::cin >> endCol;
	std::cout << std::endl;
	std::cout << "Row : ";
	std::cin >> endRow;

	return Move(Square(startCol, startRow), Square(endCol, endRow));
}
