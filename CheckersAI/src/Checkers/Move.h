#pragma once
#include <iostream>

#include "Square.h"

struct Move
{
	Position from;
	Position to;

	Move(Position start, Position end)
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

