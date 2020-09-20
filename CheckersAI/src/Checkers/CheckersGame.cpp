#include "CheckersGame.h"

#include <iostream>
#include <cmath>

#include "Move.h"
#include "Square.h"


CheckersGame::CheckersGame(Color manualPlayersColor)
{
	//if (manualPlayersColor == WHITE)
	//{
	//	whitePlayer = std::make_unique<ManualPlayer>(WHITE);
	//	blackPlayer = std::make_unique<AIPlayer>(BLACK);
	//}
	//else
	//{
	//	whitePlayer = std::make_unique<AIPlayer>(WHITE);
	//	blackPlayer = std::make_unique<ManualPlayer>(BLACK);
	//}

	whitePlayer = std::make_unique<ManualPlayer>(WHITE);
	blackPlayer = std::make_unique<ManualPlayer>(BLACK);

	turn = BLACK;
}


void CheckersGame::initialize()
{
	turn = BLACK;

	board.initialize(*blackPlayer, *whitePlayer);

	//std::shared_ptr<Piece> newPiece1 = std::make_shared<Piece>(BLACK);
	//board[0][0].occupent = newPiece1;
	//blackPlayer->addPiece(newPiece1);

	//std::shared_ptr<Piece> newPiece2 = std::make_shared<Piece>(WHITE);
	//board[0][1].occupent = newPiece2;
	//whitePlayer->addPiece(newPiece2);

	//std::shared_ptr<Piece> newPiece3 = std::make_shared<Piece>(WHITE);
	//board[3][3].occupent = newPiece3;
	//whitePlayer->addPiece(newPiece3);
}


void CheckersGame::play()
{
	initialize();

	bool gameOver = false;

	int turns = 1;

	while (!gameOver)
	{
		board.print();
		std::cout << "\n";

		std::vector<Move> validMoves = getValidMoves();

		if (validMoves.empty())
		{
			gameOver = true;
			winningColor = (turn == WHITE) ? BLACK : WHITE;
			break;
		}

		if (turn == WHITE)
		{
			//if (turns == 2)
			//{
			//	Move move = validMoves[5];
			//	board.executeMove(move);
			//}
			//else
			//{

			//	Move move = whitePlayer->chooseMove(board, validMoves);
			//	board.executeMove(move);
			//}

			Move move = whitePlayer->chooseMove(board, validMoves);
			board.executeMove(move);
			turn = BLACK;
		}
		else if (turn == BLACK)
		{
			//if (turns == 1)
			//{
			//	Move move = validMoves[3];

			//	board.executeMove(move);
			//}
			//else if (turns == 3)
			//{
			//	Move move = validMoves[0];
			//	board.executeMove(move);
			//}
			//else
			//{
			//	Move move = blackPlayer->chooseMove(board, validMoves);

			//	board.executeMove(move);
			//}

			Move move = blackPlayer->chooseMove(board, validMoves);
			board.executeMove(move);

			turn = WHITE;
		}
		checkIfGameOver(gameOver);

		turns++;
	}
}


std::vector<Move> CheckersGame::getValidMoves()
{
	std::vector<Move> validmoves;
	std::vector<Move> validJumps;
	Player& currentPlayer = (turn == WHITE) ? *whitePlayer : *blackPlayer;
	

	for (int i = 0; i < 12; i++)
	{
		auto piece = currentPlayer.getPiece(i);
		if (piece != NULL)
		{
			board.getNormalMoves(piece, validmoves);
			board.getJumpMoves(piece, validJumps);
		}
	}
	if (!validJumps.empty())
	{  
		return validJumps;
	}
	return validmoves;
}


void CheckersGame::checkIfGameOver(bool& gameOver)
{
	if (whitePlayer->isEmpty())
	{
		gameOver = true;
		winningColor = WHITE;
	}
	else if (blackPlayer->isEmpty())
	{
		gameOver = false;
		winningColor = BLACK;
	}
}
