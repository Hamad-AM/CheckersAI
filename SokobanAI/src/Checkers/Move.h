#pragma once
#include "Square.h"

#include <iostream>

struct Move
{
	Square from;
	Square to;

	Move(Square start, Square end)
		: from(start), to(end)
	{
	}

	Move(const Move& move)
	{
		from = move.from;
		to = move.to;
	}

	void write() const
	{
		std::cout << "From : ";
		from.write();
		std::cout << " To : ";
		to.write();
	}
};

