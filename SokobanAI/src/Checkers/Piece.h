#pragma once

#include <string>

#include "Color.h"

struct Piece
{
	Color color;
	bool king = false;

	Piece(Color colorIn) 
		: color(colorIn) {}
};