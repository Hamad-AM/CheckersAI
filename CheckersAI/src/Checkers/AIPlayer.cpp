#include <limits>
#include <algorithm>

#include "AIPlayer.h"

AIPlayer::AIPlayer(Color colorIn)
	: Player(colorIn)
{
	
}

Move AIPlayer::chooseMove(Board board, std::vector<Move> validMoves)
{
	// create nodes at a depth of 50
	GameState rootState = GameState(board, 0, Move(Position(0, 1), Position(0, 1)));

	createNodes(rootState, depth, true);

	// get next best move
	return minmax(rootState, true, 0).move;
}

void AIPlayer::createNodes(GameState& node, int currentDepth, bool isMaxPlayer)
{
	if (!isTerminalState(node) && currentDepth > 0)
	{
		std::vector<Move> validMoves;
		if (isMaxPlayer)
		{
			validMoves = getValidMovesFor(node.board, color);
		}
		else
		{
			Color otherColor = color == WHITE ? BLACK : WHITE;
			validMoves = getValidMovesFor(node.board, otherColor);
		}

		if (validMoves.empty())
		{
			return;
		}
		else
		{

			std::vector<int> values;
			for (Move move : validMoves)
			{
				// Create new board state
				DataType boardData = node.board.getBoardData();
				Board newBoard = node.board;
				newBoard.executeMove(move);

				// Create new game node
				int value = evaluateState(newBoard);
				values.push_back(value);
				bool isViable = false;
				
				if (isMaxPlayer)
				{
					int maxValue = *std::max_element(values.begin(), values.end());
					if (value < maxValue)
					{
						isViable = false;
					}
					else
					{
						isViable = true;
					}
				}
				else
				{
					int minValue = *std::min_element(values.begin(), values.end());
					if (value > minValue)
					{
						isViable = false;
					}
					else
					{
						isViable = true;
					}
				}

				if (isViable)
				{
					GameState newState(newBoard, value, move);
					node.children.push_back(newState);
				}
			}

			int nodesdepth = currentDepth - 1;

			for (int i = 0; i < node.children.size(); i++)
			{
				createNodes(node.children[i], nodesdepth, !isMaxPlayer);
			}
		}
	}
	else
	{
		return;
	}

}

std::vector<Move> AIPlayer::getValidMovesFor(Board& board, Color colorsmove)
{
	std::vector<std::weak_ptr<Piece>> pieces = board.getColorsPieces(colorsmove);

	std::vector<Move> validMoves;
	std::vector<Move> validJumps;

	for (std::weak_ptr<Piece> piece : pieces)
	{
		auto piece_sp = piece.lock();
		board.getNormalMoves(piece_sp, validMoves);
		board.getJumpMoves(piece_sp, validJumps);
	}
	if (!validJumps.empty())
	{
		return validJumps;
	}

	return validMoves;
}

bool AIPlayer::isTerminalState(GameState node)
{
	Board board = node.board;
	if (board.getColorsPieces(WHITE).empty() || board.getColorsPieces(BLACK).empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
AI player is always max
Return best option move
*/
GameState AIPlayer::minmax(GameState& node, bool isMaxPlayer, int currentDepth)
{
	if (currentDepth >= depth)
	{
		return node;
	}
	if (isMaxPlayer)
	{
		constexpr int currentValue = std::numeric_limits<int>().min();
		int bestNodeIndex = -1;
		for (int i = 0; i < node.children.size(); i++)
		{
			if (currentValue <= minmax(node.children[i], false, currentDepth + 1).label)
			{
				bestNodeIndex = i;
			}
		}
		return node.children[bestNodeIndex];
	}
	else
	{
		constexpr int currentValue = std::numeric_limits<int>().max();
		int bestNodeIndex = -1;
		for (int i = 0; i < node.children.size(); i++)
		{
			if (currentValue >= minmax(node.children[i], false, currentDepth + 1).label)
			{
				bestNodeIndex = i;
			}
		}
		return node.children[bestNodeIndex];
	}
}

int AIPlayer::evaluateState(Board& board)
{
	Color otherColor = color == WHITE ? BLACK : WHITE;
	return board.getColorsPieces(color).size() - board.getColorsPieces(otherColor).size();
}

void AIPlayer::printNodes(GameState& rootNode) const
{
	//rootNode.board.print();

	for (GameState state : rootNode.children)
	{
		state.board.print();
	}

	for (GameState state : rootNode.children)
	{
		printNodes(state);
	}
}