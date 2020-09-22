#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Piece.h"
#include "Color.h"
#include "Move.h"

class Board;

class Player
{
public:
	Player(Color colorIn);

	void addPiece(std::weak_ptr<Piece> piece);
	std::shared_ptr<Piece> getPiece(int i) { return pieces[i].lock(); }
	bool isEmpty();
	int getNumOfPieces();

	virtual Move chooseMove(Board board, std::vector<Move> validMoves) = 0;

protected:
	Color color;

private:
	std::vector<std::weak_ptr<Piece>> pieces;
	std::weak_ptr<Square> pos;
};
