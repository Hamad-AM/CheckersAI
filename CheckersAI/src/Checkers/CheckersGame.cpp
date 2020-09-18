#include "CheckersGame.h"

#include <iostream>
#include <cmath>

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

			//if ((i + j) % 2 != 0)
			//{
			//	if (j < 3)
			//	{
			//		std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(BLACK);
			//		newSquare.occupent = newPiece;
			//		blackPlayer->addPiece(newPiece);
			//	}
			//	else if (j > 4)
			//	{
			//		std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(WHITE);
			//		newSquare.occupent = newPiece;
			//		whitePlayer->addPiece(newPiece);
			//	}
			//}
			board[i][j] = newSquare;
		}
	}

	std::shared_ptr<Piece> newPiece1 = std::make_shared<Piece>(BLACK);
	board[0][0].occupent = newPiece1;
	blackPlayer->addPiece(newPiece1);

	std::shared_ptr<Piece> newPiece2 = std::make_shared<Piece>(WHITE);
	board[0][1].occupent = newPiece2;
	whitePlayer->addPiece(newPiece2);

	//std::shared_ptr<Piece> newPiece3 = std::make_shared<Piece>(WHITE);
	//board[3][3].occupent = newPiece3;
	//whitePlayer->addPiece(newPiece3);
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
	std::vector<Move> validJumps;
	Player& currentPlayer = (turn == WHITE) ? *whitePlayer : *blackPlayer;
	
	for (int i = 0; i < currentPlayer.getNumOfPieces(); i++)
	{
		auto piece = currentPlayer.getPiece(i);
		if (piece != NULL)
		{
			getNormalMoves(piece, validmoves);
			getJumpMoves(piece, validJumps);
		}
	}
	if (!validJumps.empty())
	{  
		return validJumps;
	}
	return validmoves;
}


void CheckersGame::getJumpMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves)
{
	Square pieceSquare = getSquareOfPiece(piece);
	std::vector<Square> squares;

	std::vector<Square> possiblePoints;
	Board boardCopy = board;
	getMultiJumpMoves(possiblePoints, pieceSquare, boardCopy, false);
	for (Square sqr : possiblePoints)
	{
		moves.emplace_back(Move(pieceSquare, sqr));
	}
}


void CheckersGame::getMultiJumpMoves(std::vector<Square>& movesSoFar, Square start, Board& board, bool isKing)
{
	if (turn == BLACK || isKing == true)
	{
		std::vector<Square> squares = getJumpSquaresDown(start, board);
		if (squares.empty())
		{
			if (!movesSoFar.empty())
			{
				movesSoFar.emplace_back(start);
				return;
			}
		}
		else
		{
			for (Square newSquare : squares)
			{
				Board newBoard = completeMove(start, newSquare, board);
				getMultiJumpMoves(movesSoFar, newSquare, newBoard, isKing);
			}
		}
	}
	if (turn == WHITE || isKing == true)
	{
		std::vector<Square> squares = getJumpSquaresUp(start, board);
		if (squares.empty())
		{
			if (!movesSoFar.empty())
			{
				movesSoFar.emplace_back(start);
				return;
			}
		}
		else
		{
			for (Square newSquare : squares)
			{
				Board newBoard = completeMove(start, newSquare, board);
				getMultiJumpMoves(movesSoFar, newSquare, newBoard, isKing);
			}
		}
	}
}


Board CheckersGame::completeMove(Square start, Square end, Board& board)
{
	board[end.col][end.row].occupent = board[start.col][start.row].occupent;
	board[(end.col - start.col) / 2][(end.row - start.row) / 2].occupent = nullptr;
	board[start.col][start.row].occupent = nullptr;
	return board;
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
	if (turn == WHITE || piece->king == true)
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


std::vector<Square> CheckersGame::getJumpSquaresDown(Square& const square, Board& board)
{
	std::vector<Square> returnSqr;
	returnSqr.reserve(4);

	if (square.col >= 2)
	{
		if (square.row <= 5)
		{
			// Check bottom left squares
			if (board[square.col - 1][square.row + 1].occupent != nullptr && board[square.col - 1][square.row + 1].occupent->color != turn)
			{
				if (board[square.col - 2][square.row + 2].occupent == nullptr)
				{
					returnSqr.emplace_back(board[square.col - 2][square.row + 2]);
				}
			}
		}
	}

	if (square.col <= 5)
	{
		if (square.row <= 5)
		{
			// Check bottom right squares
			if (board[square.col + 1][square.row + 1].occupent != nullptr 
				&& board[square.col + 1][square.row + 1].occupent->color != turn)
			{
				if (board[square.col + 2][square.row + 2].occupent == nullptr)
				{
					returnSqr.emplace_back(board[square.col + 2][square.row + 2]);
				}
			}
		}
	}

	return returnSqr;
}


std::vector<Square> CheckersGame::getJumpSquaresUp(Square& const square, Board& board)
{
	std::vector<Square> returnSqr;
	returnSqr.reserve(4);

	if (square.col >= 2)
	{
		if (square.row >= 2)
		{
			// Check top left squares
			if (board[square.col - 1][square.row - 1].occupent != nullptr 
				&& board[square.col - 1][square.row - 1].occupent->color != turn)
			{
				if (board[square.col - 2][square.row - 2].occupent == nullptr)
				{
					returnSqr.emplace_back(board[square.col + 2][square.row + 2]);
				}
			}
		}
	}

	if (square.col <= 5)
	{
		if (square.row >= 2)
		{
			// Check the top right squares
			if (board[square.col + 1][square.row - 1].occupent != nullptr 
				&& board[square.col + 1][square.row + 1].occupent->color != turn)
			{
				if (board[square.col + 2][square.row - 2].occupent == nullptr)
				{
					returnSqr.emplace_back(board[square.col + 2][square.row + 2]);
				}
			}
		}
	}
	return returnSqr;
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
	Square& start = move.from;
	Square& end = move.to;
	// Check if move is jump
	int diff = std::abs(end.col - start.col);
	if (diff > 1)
	{
		// Check how many jumps there are
		board[end.col][end.row].occupent = board[start.col][start.row].occupent;
		board[start.col][start.row].occupent = nullptr;
		// Delete pieces jumped over
		for (int i = 1; i < std::abs(end.col - start.col); i = i + 2)
		{
			board[start.col + ((end.col - start.col) / diff) * i][start.row + ((end.row - start.row) / diff) * i].occupent = nullptr;
		}
	}
	else
	{
		// if not complete normal move
		board[move.to.col][move.to.row].occupent = board[move.from.col][move.from.row].occupent;
		board[move.from.col][move.from.row].occupent = nullptr;
	}
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
