#pragma once

#include <memory>
#include <iostream>

#include "Piece.h"

struct Square
{
	int col;
	int row;
	std::shared_ptr<Piece> occupent;

	Square()
	{
		col = -1;
		row = -1;
	}
	
	Square(int colIn, int rowIn)
		: col(colIn), row(rowIn), occupent(nullptr) {}

	Square(const Square& square)
	{
		col = square.col;
		row = square.row;
		occupent = square.occupent;
	}

	void write() const
	{
		std::cout << "[" << col << ", " << row << "]";
	}

	bool operator==(Square& rhs)
	{
		if (this->col == rhs.col && this->row == rhs.row)
		{
			return true;
		}

		return false;
	}
};
