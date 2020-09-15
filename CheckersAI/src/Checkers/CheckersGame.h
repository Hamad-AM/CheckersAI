#pragma once

#include <memory>
#include <vector>

#include "Square.h"
#include "Player.h"
#include "ManualPlayer.h"
#include "AIPlayer.h"


class CheckersGame
{
public:
	CheckersGame(Color manualPlayersColor);

	void play();
private:
	void initialize();
	void checkIfGameOver(bool& gameOver);
	void executeMove(Move& const move);
	std::vector<Move> getValidMoves();
	void getNormalMoves(std::shared_ptr<Piece> const piece, std::vector<Move>& moves);
	std::vector<Square> getDiagonalSquaresDown(Square& const relativeSquare);
	std::vector<Square> getDiagonalSquaresUp(Square& const relativeSquare);

	Square getSquareOfPiece(std::shared_ptr<Piece> const piece);
	Square getTopLeft(Square& const relativeSquare);
	Square getTopRight(Square& const relativeSquare);
	Square getBottomRight(Square& const relativeSquare);
	Square getBottomLeft(Square& const relativeSquare);

	void printboard();

	Color turn;
	Color winningColor;

	std::vector<std::vector<Square>> board;

	std::unique_ptr<Player> whitePlayer;
	std::unique_ptr<Player> blackPlayer;
};

