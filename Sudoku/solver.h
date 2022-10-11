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

        for (int i = 0; i < 81; ++i)
        {
            char ch = board->readCell(i);
            if (ch != '-')
                metaState->setCell(i, ch - '1');
        }
        // metaState->listIncomplete();

        int iterations = 0;
        while (metaState->incomplete() && iterations < 5)
        {
            // single possiblity setting
            for (int i = 0; i < 81; ++i)
                if (!metaState->set[i])
                    if (metaState->getPossibleSize(i) == 1)
                    {
                        int index = metaState->getSingle(i);
                        game->setCell(i / 9, i % 9, index + 1);
                        metaState->setCell(i, index);
                    }
            iterations++;
        }
        return iterations;
    }
};
