#include "Board.h"

#include "Player.h"

Board::Board()
	: data(8, std::vector<Square>(8, Square(0, 0)))
{
	
}

void Board::initialize(Player& blackPlayer, Player& whitePlayer)
{
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
					blackPlayer.addPiece(newPiece);
				}
				else if (j > 4)
				{
					std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(WHITE);
					newSquare.occupent = newPiece;
					whitePlayer.addPiece(newPiece);
				}
			}
			data[i][j] = newSquare;
		}
	}

	//std::shared_ptr<Piece> newPiece = std::make_shared<Piece>(WHITE);
	//data[7][6].occupent = newPiece;
	//whitePlayer.addPiece(newPiece);

	//std::shared_ptr<Piece> newPiece2 = std::make_shared<Piece>(WHITE);
	//data[5][6].occupent = newPiece2;
	//whitePlayer.addPiece(newPiece2);

	//std::shared_ptr<Piece> newPiece3 = std::make_shared<Piece>(BLACK);
	//data[6][5].occupent = newPiece3;
	//blackPlayer.addPiece(newPiece3);
}

void Board::executeMove(Move& const move)
{
	Square& start = move.from;
	Square& end = move.to;

	movePiece(start, end);

	// Make piece king if end of board reached
	if (data[end.col][end.row].occupent->color == WHITE)
	{
		if (end.row == 0)
		{
			data[end.col][end.row].occupent->king = true;
		}
	}
	else
	{
		if (end.row == 7)
		{
			data[end.col][end.row].occupent->king = true;
		}
	}

	// delete captured pieces if jump was performed
	int diff = std::abs(end.col - start.col);
	if (diff > 1)
	{
		removeCapturedPieces(diff, start, end);
	}
}

void Board::movePiece(Square& const start, Square& const end)
{
	data[end.col][end.row].occupent = data[start.col][start.row].occupent;
	data[start.col][start.row].occupent = nullptr;
}

void Board::removeCapturedPieces(int& const noJumps, Square& const start, Square& const end)
{
	for (int i = 1; i < noJumps; i = i + 2)
	{
		data[start.col + ((end.col - start.col) / noJumps) * i][start.row + ((end.row - start.row) / noJumps) * i].occupent = nullptr;
	}
}

std::vector<Square> Board::getDiagonalSquaresDown(Square& const relativeSquare)
{
	std::vector<Square> squares;
	squares.reserve(2);

	//if (relativeSquare.col == 7 && relativeSquare.row == 6)
	//{
	//	std::cout << "Problem here" << std::endl;
	//}

	// Get bottom left square if not in bottom left corner
	if (relativeSquare.col > 0)
	{
		if (relativeSquare.row < 7)
		{
			squares.emplace_back(getBottomLeft(relativeSquare));
		}
	}

	// Get bottom right square if not in bottom right corner
	if (relativeSquare.col < 7)
	{
		if (relativeSquare.row < 7)
		{
			squares.emplace_back(getBottomRight(relativeSquare));
		}
	}

	return squares;
}

std::vector<Square> Board::getDiagonalSquaresUp(Square& const relativeSquare)
{
	std::vector<Square> squares;
	squares.reserve(2);

	// Get top left square if not in the top left corner
	if (relativeSquare.col > 0)
	{
		if (relativeSquare.row > 0)
		{
			squares.emplace_back(getTopLeft(relativeSquare));
		}
	}

	if (relativeSquare.col < 7)
	{
		if (relativeSquare.row > 0)
		{
			squares.emplace_back(getTopRight(relativeSquare));
		}
	}

	return squares;
}

void Board::getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves)
{

	Square pieceSquare = getSquareOfPiece(piece);

	//if ((pieceSquare.col == 7) && (pieceSquare.row == 6))
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	std::vector<Square> squares;
	if (piece->color == BLACK || piece->king == true)
	{
		for (Square square : getDiagonalSquaresDown(pieceSquare))
		{
			squares.push_back(square);
		}
	}
	if (piece->color == WHITE || piece->king == true)
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

struct multi_jumps
{
	std::vector<Square> jumps;
	multi_jumps(Square start, DataType dataCopy, Color color, bool king)
	{
		jumps.reserve(10);
		explore({}, start, dataCopy, color, king);
	}

	void explore(std::vector<Square> so_far, Square start, DataType dataCopy, Color color, bool king)
	{
		auto moves = Board::possible_jumps(start, dataCopy, color, king);

		if (moves.empty()) {
			if (!so_far.empty()) {
				for (Square square : so_far)
				{
					jumps.push_back(square);
				}
			}
		}
		else {
			for (const auto move : moves) {
				DataType new_board = Board::completeMove(start, move, dataCopy);
				std::vector<Square> new_so_far = so_far;
				new_so_far.push_back(move);
				explore(new_so_far, move, new_board, color, king);
			}
		}
	}
};

void Board::getJumpMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves)
{
	Square pieceSquare = getSquareOfPiece(piece);
	std::vector<Square> squares;

	//if ((pieceSquare.col == 7) && (pieceSquare.row == 6))
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	std::vector<Square> possiblePoints;
	DataType boardCopy = data;
	//getMultiJumpMoves(*piece, possiblePoints, pieceSquare, boardCopy);
	possiblePoints = multi_jumps(pieceSquare, boardCopy, piece->color, piece->king).jumps;
	for (Square sqr : possiblePoints)
	{
		moves.emplace_back(Move(pieceSquare, sqr));
	}
}

std::vector<Square> Board::possible_jumps(Square start, DataType dataCopy, Color color, bool king)
{
	std::vector<Square> squares;
	if (color == BLACK || king == true)
	{
		squares = getJumpSquaresDown(start, dataCopy, color);
	}
	if (color == WHITE || king == true)
	{
		squares = getJumpSquaresUp(start, dataCopy, color);

	}

	return squares;
}

void Board::getMultiJumpMoves(Piece& const piece, std::vector<Square>& movesSoFar, Square start, DataType& dataCopy)
{
	if (piece.color == BLACK || piece.king == true)
	{
		std::vector<Square> squares = getJumpSquaresDown(start, dataCopy, piece.color);

		getFurtherJumps(piece, squares, movesSoFar, start, dataCopy);
	}
	if (piece.color == WHITE || piece.king == true)
	{
		std::vector<Square> squares = getJumpSquaresUp(start, dataCopy, piece.color);

		getFurtherJumps(piece, squares, movesSoFar, start, dataCopy);
	}
}

void Board::getFurtherJumps(Piece& const piece, std::vector<Square>& squares, std::vector<Square>& movesSoFar, Square start, DataType& dataCopy)
{
	//if (start.col == 4 && start.row == 3)
	//{
	//	std::cout << "Debug point" << std::endl;
	//}

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
			DataType newBoard = completeMove(start, newSquare, dataCopy);
			std::vector<Square> newMovesSoFar = movesSoFar;
			newMovesSoFar.push_back(newSquare);
			getMultiJumpMoves(piece, newMovesSoFar, newSquare, newBoard);
		}
	}
}

Square Board::getSquareOfPiece(std::shared_ptr<Piece> const piece)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (data[i][j].occupent != nullptr)
			{
				if (data[i][j].occupent == piece)
				{
					return data[i][j];
				}
			}
		}
	}
}

Square Board::getTopLeft(Square& const relativeSquare)
{
	return data[relativeSquare.col - 1][relativeSquare.row - 1];
}

Square Board::getTopRight(Square& const relativeSquare)
{
	return data[relativeSquare.col + 1][relativeSquare.row - 1];
}

Square Board::getBottomRight(Square& const relativeSquare)
{
	return data[relativeSquare.col + 1][relativeSquare.row + 1];
}

Square Board::getBottomLeft(Square& const relativeSquare)
{
	return data[relativeSquare.col - 1][relativeSquare.row + 1];
}

DataType Board::completeMove(Square start, Square end, DataType& dataCopy)
{
	//if ((end.col == 7) && (end.row = 4))
	//{
	//	std::cout << start.col + (end.col - start.col) / 2 << ", " << start.row + (end.row - start.row) / 2 << std::endl;
	//}

	dataCopy[end.col][end.row].occupent = dataCopy[start.col][start.row].occupent;
	dataCopy[start.col + (end.col - start.col) / 2][start.row + (end.row - start.row) / 2].occupent = nullptr;
	dataCopy[start.col][start.row].occupent = nullptr;
	return dataCopy;
}

std::vector<Square> Board::getJumpSquaresDown(Square square, DataType dataCopy, Color color)
{
	std::vector<Square> returnSqr;
	returnSqr.reserve(4);
	
	if (square.col >= 2)
	{
		if (square.row <= 5)
		{
			// Check bottom left squares
			if (dataCopy[square.col - 1][square.row + 1].occupent != nullptr 
				&& dataCopy[square.col - 1][square.row + 1].occupent->color != color)
			{
				if (dataCopy[square.col - 2][square.row + 2].occupent == nullptr)
				{
					/*std::cout << square.col - 2 << ", " << square.row + 2 << std::endl;*/
					returnSqr.emplace_back(dataCopy[square.col - 2][square.row + 2]);
				}
			}
		}
	}

	if (square.col <= 5)
	{
		if (square.row <= 5)
		{
			// Check bottom right squares
			if (dataCopy[square.col + 1][square.row + 1].occupent != nullptr
				&& dataCopy[square.col + 1][square.row + 1].occupent->color != color)
			{
				if (dataCopy[square.col + 2][square.row + 2].occupent == nullptr)
				{

					/*std::cout << square.col + 2 << ", " << square.row + 2 << std::endl;*/
					returnSqr.emplace_back(dataCopy[square.col + 2][square.row + 2]);
				}
			}
		}
	}

	return returnSqr;
}

std::vector<Square> Board::getJumpSquaresUp(Square square, DataType dataCopy, Color color)
{
	std::vector<Square> returnSqr;
	returnSqr.reserve(4);

	//if (square.col == 7 && square.row == 6)
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	if (square.col >= 2)
	{
		if (square.row >= 2)
		{
			// Check top left squares
			if (dataCopy[square.col - 1][square.row - 1].occupent != nullptr
				&& dataCopy[square.col - 1][square.row - 1].occupent->color != color)
			{
				if (dataCopy[square.col - 2][square.row - 2].occupent == nullptr)
				{

					/*std::cout << square.col - 2 << ", " << square.row - 2 << std::endl;*/
					returnSqr.push_back(dataCopy[square.col - 2][square.row - 2]);
				}
			}
		}
	}

	if (square.col <= 5)
	{
		if (square.row >= 2)
		{
			// Check the top right squares
			if (dataCopy[square.col + 1][square.row - 1].occupent != nullptr
				&& dataCopy[square.col + 1][square.row - 1].occupent->color != color)
			{
				if (dataCopy[square.col + 2][square.row - 2].occupent == nullptr)
				{

					/*std::cout << square.col + 2 << ", " << square.row - 2 << std::endl;
					if (((square.col + 2) == 7) && ((square.row - 2) == 4))
					{
						dataCopy[7][4].write();
					}*/
					returnSqr.push_back(dataCopy[square.col + 2][square.row - 2]);
				}
			}
		}
	}
	return returnSqr;
}

void Board::print()
{
	std::cout << "  |0| |1| |2| |3| |4| |5| |6| |7|\n";
	for (int j = 0; j < 8; j++)
	{
		std::cout << j << " ";

		for (int i = 0; i < 8; i++)
		{
			std::cout << "|";
			if (data[i][j].occupent != nullptr)
			{
				if (data[i][j].occupent->color == WHITE)
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
		std::cout << "\n";
	}
}
