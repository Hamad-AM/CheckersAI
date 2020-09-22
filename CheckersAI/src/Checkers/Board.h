#pragma once

#include <vector>

#include "Square.h"
#include "Move.h"
#include "Position.h"

using DataType = std::vector<std::vector<Square>>;

class Player;

class Board
{
public:
	Board();

	void initialize(Player& blackPlayer, Player& whitePlayer);

	void executeMove(Move& const move);

	std::vector<Square> getDiagonalSquaresDown(Position& const relativePosition) const;
	std::vector<Square> getDiagonalSquaresUp(Position& const relativePosition) const;

	void getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves) const;
	void getJumpMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves) const;
	void getMultiJumpMoves(Piece& const piece, std::vector<Position>& movesSoFar, Position start, DataType& dataCopy);
	void getFurtherJumps(Piece& const piece, std::vector<Position>& positions, std::vector<Position>& movesSoFar, Position start, DataType& dataCopy);

	Position getPositionOfPiece(std::shared_ptr<Piece> const piece) const;
	Position getTopLeft(Position& const relativePosition) const;
	Position getTopRight(Position& const relativePosition) const;
	Position getBottomRight(Position& const relativePosition) const;
	Position getBottomLeft(Position& const relativePosition) const;

	static DataType completeMove(Position start, Position end, DataType& dataCopy);
	static std::vector<Position> getJumpPositionsDown(Position position, DataType dataCopy, Color color);
	static std::vector<Position> getJumpPositionsUp(Position position, DataType dataCopy, Color color);
	static std::vector<Position> possible_jumps(Position start, DataType dataCopy, Color color, bool king);

	std::vector<std::weak_ptr<Piece>> getColorsPieces(Color color);
	DataType getBoardData() const;

	void print();
private:
	void movePiece(Position& const start, Position& const end);
	void removeCapturedPieces(int& const noJumps, Position& const start, Position& const end);

	DataType data;
};

