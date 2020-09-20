#pragma once

#include <vector>

#include "Square.h"
#include "Move.h"

using DataType = std::vector<std::vector<Square>>;

class Player;

class Board
{
public:
	Board();

	void initialize(Player& blackPlayer, Player& whitePlayer);

	void executeMove(Move& const move);

	std::vector<Square> getDiagonalSquaresDown(Square& const relativeSquare);
	std::vector<Square> getDiagonalSquaresUp(Square& const relativeSquare);

	void getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves);
	void getJumpMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves);
	void getMultiJumpMoves(Piece& const piece, std::vector<Square>& movesSoFar, Square start, DataType& dataCopy);
	void getFurtherJumps(Piece& const piece, std::vector<Square>& squares, std::vector<Square>& movesSoFar, Square start, DataType& dataCopy);

	Square getSquareOfPiece(std::shared_ptr<Piece> const piece);
	Square getTopLeft(Square& const relativeSquare);
	Square getTopRight(Square& const relativeSquare);
	Square getBottomRight(Square& const relativeSquare);
	Square getBottomLeft(Square& const relativeSquare);

	static DataType completeMove(Square start, Square end, DataType& dataCopy);
	static std::vector<Square> getJumpSquaresDown(Square square, DataType dataCopy, Color color);
	static std::vector<Square> getJumpSquaresUp(Square square, DataType dataCopy, Color color);
	static std::vector<Square> possible_jumps(Square start, DataType dataCopy, Color color, bool king);


	void print();
private:
	void movePiece(Square& const start, Square& const end);
	void removeCapturedPieces(int& const noJumps, Square& const start, Square& const end);

	DataType data;
};

