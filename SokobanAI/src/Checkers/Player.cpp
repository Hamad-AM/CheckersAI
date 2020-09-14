#include "Player.h"

Player::Player(Color colorIn)
    : color(colorIn)
{
}

void Player::addPiece(std::weak_ptr<Piece> piece)
{
    pieces.emplace_back(piece);
}


int Player::getNumOfPieces()
{
    int count = 0;
    for (int i = 0; i < pieces.size(); i++)
    {
        if (auto piece = pieces[i].lock())
        {
            count++;
        }
    }

    return count;
}

bool Player::isEmpty()
{
    return (getNumOfPieces() <= 0) ? true : false;
}
