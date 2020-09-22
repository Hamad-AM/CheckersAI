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
	//data[7][7].occupent = newPiece;
	//whitePlayer.addPiece(newPiece);

	//std::shared_ptr<Piece> newPiece2 = std::make_shared<Piece>(WHITE);
	//data[6][6].occupent = newPiece2;
	//whitePlayer.addPiece(newPiece2);

	//std::shared_ptr<Piece> newPiece3 = std::make_shared<Piece>(BLACK);
	//data[1][1].occupent = newPiece3;
	//blackPlayer.addPiece(newPiece3);


	//std::shared_ptr<Piece> newPiece4 = std::make_shared<Piece>(BLACK);
	//data[0][0].occupent = newPiece4;
	//blackPlayer.addPiece(newPiece4);
}

void Board::executeMove(Move& const move)
{
	Position& start = move.from;
	Position& end = move.to;

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

void Board::movePiece(Position& const start, Position& const end)
{
	data[end.col][end.row].occupent = data[start.col][start.row].occupent;
	data[start.col][start.row].occupent = nullptr;
}

void Board::removeCapturedPieces(int& const noJumps, Position& const start, Position& const end)
{
	for (int i = 1; i < noJumps; i = i + 2)
	{
		data[start.col + ((end.col - start.col) / noJumps) * i][start.row + ((end.row - start.row) / noJumps) * i].occupent = nullptr;
	}
}

std::vector<Square> Board::getDiagonalSquaresDown(Position& const relativePosition) const
{
	std::vector<Square> positions;
	positions.reserve(2);

	//if (relativePosition.col == 7 && relativePosition.row == 6)
	//{
	//	std::cout << "Problem here" << std::endl;
	//}

	// Get bottom left position if not in bottom left corner
	if (relativePosition.col > 0)
	{
		if (relativePosition.row < 7)
		{
			Position pos = getBottomLeft(relativePosition);
			positions.emplace_back(data[pos.col][pos.row]);
		}
	}

	// Get bottom right position if not in bottom right corner
	if (relativePosition.col < 7)
	{
		if (relativePosition.row < 7)
		{
			Position pos = getBottomRight(relativePosition);
			positions.emplace_back(data[pos.col][pos.row]);
		}
	}

	return positions;
}

std::vector<Square> Board::getDiagonalSquaresUp(Position& const relativePosition) const
{
	std::vector<Square> squares;
	squares.reserve(2);

	// Get top left position if not in the top left corner
	if (relativePosition.col > 0)
	{
		if (relativePosition.row > 0)
		{
			Position pos = getTopLeft(relativePosition);
			squares.emplace_back(data[pos.col][pos.row]);
		}
	}

	if (relativePosition.col < 7)
	{
		if (relativePosition.row > 0)
		{
			Position pos = getTopRight(relativePosition);
			squares.emplace_back(data[pos.col][pos.row]);
		}
	}

	return squares;
}

void Board::getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves) const
{

	Position piecePosition = getPositionOfPiece(piece);

	//if ((piecePosition.col == 7) && (piecePosition.row == 6))
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	std::vector<Square> squares;
	if (piece->color == BLACK || piece->king == true)
	{
		for (Square square : getDiagonalSquaresDown(piecePosition))
		{
			squares.push_back(square);
		}
	}
	if (piece->color == WHITE || piece->king == true)
	{
		for (Square square : getDiagonalSquaresUp(piecePosition))
		{
			squares.push_back(square);
		}
	}

	for (Square square : squares)
	{
		if (square.occupent == nullptr)
		{
			moves.emplace_back<Move>(Move(piecePosition, Position(square.col, square.row)));
		}
	}
}

struct multi_jumps
{
	std::vector<Position> jumps;
	multi_jumps(Position start, DataType dataCopy, Color color, bool king)
	{
		jumps.reserve(10);
		explore({}, start, dataCopy, color, king);
	}

	void explore(std::vector<Position> so_far, Position start, DataType dataCopy, Color color, bool king)
	{
		auto moves = Board::possible_jumps(start, dataCopy, color, king);

		if (moves.empty()) {
			if (!so_far.empty()) {
				for (Position position : so_far)
				{
					jumps.push_back(position);
				}
			}
		}
		else {
			for (const auto move : moves) {
				DataType new_board = Board::completeMove(start, move, dataCopy);
				std::vector<Position> new_so_far = so_far;
				new_so_far.push_back(move);
				explore(new_so_far, move, new_board, color, king);
			}
		}
	}
};

void Board::getJumpMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves) const
{
	Position piecePosition = getPositionOfPiece(piece);
	std::vector<Position> positions;

	//if ((piecePosition.col == 7) && (piecePosition.row == 6))
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	std::vector<Position> possiblePoints;
	DataType boardCopy = data;
	//getMultiJumpMoves(*piece, possiblePoints, piecePosition, boardCopy);
	possiblePoints = multi_jumps(piecePosition, boardCopy, piece->color, piece->king).jumps;
	for (Position sqr : possiblePoints)
	{
		moves.emplace_back(Move(piecePosition, sqr));
	}
}

std::vector<Position> Board::possible_jumps(Position start, DataType dataCopy, Color color, bool king)
{
	std::vector<Position> positions;
	if (color == BLACK || king == true)
	{
		positions = getJumpPositionsDown(start, dataCopy, color);
	}
	if (color == WHITE || king == true)
	{
		positions = getJumpPositionsUp(start, dataCopy, color);

	}

	return positions;
}

std::vector<std::weak_ptr<Piece>> Board::getColorsPieces(Color color)
{
	std::vector<std::weak_ptr<Piece>> outPieces;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (data[i][j].occupent != nullptr)
			{
				if (data[i][j].occupent->color == color)
				{
					outPieces.push_back(data[i][j].occupent);
				}
			}
		}
	}
	return outPieces;
}

DataType Board::getBoardData() const
{
	return data;
}

void Board::getMultiJumpMoves(Piece& const piece, std::vector<Position>& movesSoFar, Position start, DataType& dataCopy)
{
	if (piece.color == BLACK || piece.king == true)
	{
		std::vector<Position> positions = getJumpPositionsDown(start, dataCopy, piece.color);

		getFurtherJumps(piece, positions, movesSoFar, start, dataCopy);
	}
	if (piece.color == WHITE || piece.king == true)
	{
		std::vector<Position> positions = getJumpPositionsUp(start, dataCopy, piece.color);

		getFurtherJumps(piece, positions, movesSoFar, start, dataCopy);
	}
}

void Board::getFurtherJumps(Piece& const piece, std::vector<Position>& positions, std::vector<Position>& movesSoFar, Position start, DataType& dataCopy)
{
	//if (start.col == 4 && start.row == 3)
	//{
	//	std::cout << "Debug point" << std::endl;
	//}

	if (positions.empty())
	{
		if (!movesSoFar.empty())
		{
			movesSoFar.emplace_back(start);
			return;
		}
	}
	else
	{
		for (Position newPosition : positions)
		{
			DataType newBoard = completeMove(start, newPosition, dataCopy);
			std::vector<Position> newMovesSoFar = movesSoFar;
			newMovesSoFar.push_back(newPosition);
			getMultiJumpMoves(piece, newMovesSoFar, newPosition, newBoard);
		}
	}
}

Position Board::getPositionOfPiece(std::shared_ptr<Piece> const piece) const
{
	Position position;
	bool found = false;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (data[i][j].occupent != nullptr)
			{
				if (data[i][j].occupent == piece)
				{
					position = Position(data[i][j].col, data[i][j].row);
					found = true;
				}
			}
		}
	}

	if (!found)
	{
		std::cout << "ERROR : Position not in board" << std::endl;
	}
	return position;
}

Position Board::getTopLeft(Position& const relativePosition) const
{
	Square square = data[relativePosition.col - 1][relativePosition.row - 1];
	return Position(square.col, square.row);
}

Position Board::getTopRight(Position& const relativePosition) const
{
	Square square = data[relativePosition.col + 1][relativePosition.row - 1];
	return Position(square.col, square.row);
}

Position Board::getBottomRight(Position& const relativePosition) const
{
	Square square = data[relativePosition.col + 1][relativePosition.row + 1];
	return Position(square.col, square.row);
}

Position Board::getBottomLeft(Position& const relativePosition) const
{
	Square square = data[relativePosition.col - 1][relativePosition.row + 1];
	return Position(square.col, square.row);
}

DataType Board::completeMove(Position start, Position end, DataType& dataCopy)
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

std::vector<Position> Board::getJumpPositionsDown(Position position, DataType dataCopy, Color color)
{
	std::vector<Position> returnSqr;
	returnSqr.reserve(4);
	
	if (position.col >= 2)
	{
		if (position.row <= 5)
		{
			// Check bottom left positions
			if (dataCopy[position.col - 1][position.row + 1].occupent != nullptr 
				&& dataCopy[position.col - 1][position.row + 1].occupent->color != color)
			{
				if (dataCopy[position.col - 2][position.row + 2].occupent == nullptr)
				{
					/*std::cout << position.col - 2 << ", " << position.row + 2 << std::endl;*/
					Square newSquare = dataCopy[position.col - 2][position.row + 2];
					returnSqr.emplace_back(Position(newSquare.col, newSquare.row));
				}
			}
		}
	}

	if (position.col <= 5)
	{
		if (position.row <= 5)
		{
			// Check bottom right positions
			if (dataCopy[position.col + 1][position.row + 1].occupent != nullptr
				&& dataCopy[position.col + 1][position.row + 1].occupent->color != color)
			{
				if (dataCopy[position.col + 2][position.row + 2].occupent == nullptr)
				{

					/*std::cout << position.col + 2 << ", " << position.row + 2 << std::endl;*/
					Square newSquare = dataCopy[position.col + 2][position.row + 2];
					returnSqr.emplace_back(Position(newSquare.col, newSquare.row));
				}
			}
		}
	}

	return returnSqr;
}

std::vector<Position> Board::getJumpPositionsUp(Position position, DataType dataCopy, Color color)
{
	std::vector<Position> returnSqr;
	returnSqr.reserve(4);

	//if (position.col == 7 && position.row == 6)
	//{
	//	std::cout << "should jump" << std::endl;
	//}

	if (position.col >= 2)
	{
		if (position.row >= 2)
		{
			// Check top left positions
			if (dataCopy[position.col - 1][position.row - 1].occupent != nullptr
				&& dataCopy[position.col - 1][position.row - 1].occupent->color != color)
			{
				if (dataCopy[position.col - 2][position.row - 2].occupent == nullptr)
				{

					/*std::cout << position.col - 2 << ", " << position.row - 2 << std::endl;*/
					Square newSquare = dataCopy[position.col - 2][position.row - 2];
					returnSqr.push_back(Position(newSquare.col, newSquare.row));
				}
			}
		}
	}

	if (position.col <= 5)
	{
		if (position.row >= 2)
		{
			// Check the top right positions
			if (dataCopy[position.col + 1][position.row - 1].occupent != nullptr
				&& dataCopy[position.col + 1][position.row - 1].occupent->color != color)
			{
				if (dataCopy[position.col + 2][position.row - 2].occupent == nullptr)
				{

					/*std::cout << position.col + 2 << ", " << position.row - 2 << std::endl;
					if (((position.col + 2) == 7) && ((position.row - 2) == 4))
					{
						dataCopy[7][4].write();
					}*/
					Square newSquare = dataCopy[position.col + 2][position.row - 2];
					returnSqr.push_back(Position(newSquare.col, newSquare.row));
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