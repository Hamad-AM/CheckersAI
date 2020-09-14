#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Piece.h"
#include "Color.h"
#include "Move.h"

class Player
{
public:
	Player(Color colorIn);

	void addPiece(std::weak_ptr<Piece> piece);
	std::weak_ptr<Piece> getPiece(int i) { return pieces[i]; }
	bool isEmpty();
	int getNumOfPieces();

	virtual Move chooseMove(std::vector<std::vector<Square>>& board, std::vector<Move> validMoves) = 0;

protected:
	Color color;

private:
	std::vector<std::weak_ptr<Piece>> pieces;
	std::weak_ptr<Square> pos;
};
