#include <stack>
#include <utility>

#include "game.h"
#include "metastate.h"
#include "../board.h"

using std::pair;
using std::stack;

class Solver
{
private:
    // ===== variables =====
    Game *game;
    stack<pair<Board, MetaState>> states;

public:
    Solver(Game *game) : game(game)
    {
        states.push(pair<Board, MetaState>{*game->getBoard(), MetaState()});
    }

    int run()
    {
        Board *board = &states.top().first;
        MetaState *metaState = &states.top().second;

        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                char ch = board->readCell(i * 9 + j);
                if (ch != '-')
                    metaState->boardSet(i, j, ch - '1');
            }

        int iterations = 0;
        while (metaState->incomplete() && iterations < 20)
        {
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    if (!metaState->set[i][j])
                    {
                        vector<int> possibles = metaState->getPossibles(i, j);
                        if (possibles.size() == 1)
                        {
                            game->setCell(i, j, possibles[0]);
                            metaState->boardSet(i, j, possibles[0] - 1);
                        }
                    }

            // metaState->listIncomplete();
            // game->print();
            iterations++;
        }
        return iterations;
    }
};
