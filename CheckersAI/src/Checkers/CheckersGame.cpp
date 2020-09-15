#include "CheckersGame.h"

#include <iostream>


#include "Move.h"
#include "Square.h"

CheckersGame::CheckersGame(Color manualPlayersColor)
	:board(8, std::vector<Square>(8, Square(0, 0)))
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

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Square newSquare(i, j);

			if ((i + j) % 2 != 0)
			{
				if (j < 3)
				{
					std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(BLACK);
					newSquare.occupent = newPiece;
					blackPlayer->addPiece(newPiece);
				}
				else if (j > 4)
				{
					std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(WHITE);
					newSquare.occupent = newPiece;
					whitePlayer->addPiece(newPiece);
				}
			}
			board[i][j] = newSquare;
		}
	}
}



void CheckersGame::play()
{
	initialize();

	bool gameOver = false;

	while (!gameOver)
	{
		printboard();
		std::vector<Move> validMoves = getValidMoves();

		if (validMoves.empty())
		{
			gameOver = true;
			winningColor = (turn == WHITE) ? BLACK : WHITE;
			break;
		}

		if (turn == WHITE)
		{
			Move move = whitePlayer->chooseMove(board, validMoves);
			executeMove(move);
			turn = BLACK;
		}
		else if (turn == BLACK)
		{
			Move move = blackPlayer->chooseMove(board, validMoves);
			executeMove(move);
			turn = WHITE;
		}

		printboard();
		checkIfGameOver(gameOver);
	}
}

std::vector<Move> CheckersGame::getValidMoves()
{
	std::vector<Move> validmoves;
	Player& currentPlayer = (turn == WHITE) ? *whitePlayer : *blackPlayer;
	
	for (int i = 0; i < currentPlayer.getNumOfPieces(); i++)
	{
		getNormalMoves(currentPlayer.getPiece(i).lock(), validmoves);
	}

	return validmoves;
}

void CheckersGame::getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves)
{
	Square pieceSquare = getSquareOfPiece(piece);
	std::vector<Square> squares;
	if (turn == BLACK || piece->king == true)
	{
		for (Square square : getDiagonalSquaresDown(pieceSquare))
		{
			squares.push_back(square);
		}
	}
	else if (turn == WHITE || piece->king == true)
	{
		for (Square square : getDiagonalSquaresUp(pieceSquare))
		{
			squares.push_back(square);
		}
	}

	for (Square square : squares)
	{
		if (square.occupent == nullptr)
		{
			moves.emplace_back<Move>(Move(pieceSquare, square));
		}
	}
}

Square CheckersGame::getSquareOfPiece(std::shared_ptr<Piece> const piece)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].occupent != nullptr)
			{
				if (board[i][j].occupent == piece)
				{
					return board[i][j];
				}
			}
		}
	}
}

std::vector<Square> CheckersGame::getDiagonalSquaresUp(Square& const relativeSquare)
{
	std::vector<Square> squares;
	squares.reserve(2);

	if (relativeSquare.col != 0)
	{
		if (relativeSquare.row != 0)
		{
			squares.emplace_back(getTopLeft(relativeSquare));
		}
	}

	if (relativeSquare.col != 7)
	{
		if (relativeSquare.row != 0)
		{
			squares.emplace_back(getTopRight(relativeSquare));
		}
	}

	return squares;
}

std::vector<Square> CheckersGame::getDiagonalSquaresDown(Square& const relativeSquare)
{
	std::vector<Square> squares;
	squares.reserve(2);


	if (relativeSquare.col != 0)
	{
		if (relativeSquare.row != 7)
		{
			squares.emplace_back(getBottomLeft(relativeSquare));
		}
	}

	if (relativeSquare.col != 7)
	{
		if (relativeSquare.row != 7)
		{
			squares.emplace_back(getBottomRight(relativeSquare));
		}
	}

	return squares;
}

Square CheckersGame::getTopLeft(Square& const relativeSquare)
{
	return board[relativeSquare.col - 1][relativeSquare.row - 1];
}


Square CheckersGame::getTopRight(Square& const relativeSquare)
{
	return board[relativeSquare.col + 1][relativeSquare.row - 1];
}


Square CheckersGame::getBottomRight(Square& const relativeSquare)
{
	return board[relativeSquare.col + 1][relativeSquare.row + 1];
}


Square CheckersGame::getBottomLeft(Square& const relativeSquare)
{
	return board[relativeSquare.col - 1][relativeSquare.row + 1];
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

void CheckersGame::executeMove(Move& const move)
{
	board[move.to.col][move.to.row].occupent = board[move.from.col][move.from.row].occupent;
	board[move.from.col][move.to.col].occupent = nullptr;
}

void CheckersGame::printboard()
{
	std::cout << "  |0| |1| |2| |3| |4| |5| |6| |7|" << std::endl;
	for (int j = 0; j < 8; j++)
	{
		std::cout << j << " ";

		for (int i = 0; i < 8; i++)
		{
			std::cout << "|";
			if (board[i][j].occupent != nullptr)
			{
				if (board[i][j].occupent->color == WHITE)
				{
					std::cout << "W";
				}
				else
				{
					std::cout << "B";
				}
			}
			else
			{
				std::cout << " ";
			}
			std::cout << "|";
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}
