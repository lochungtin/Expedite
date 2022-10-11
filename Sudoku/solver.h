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

    void frequencyAnalysis(int index, MetaState *metaState)
    {
        // frequency table
        int rFreq[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int cFreq[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int sFreq[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

        // position table
        int rPos[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int cPos[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int sPos[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

        // absolute values
        int row = (index / 9) * 9;
        int col = index % 9;

        // scanning
        for (int value = 0; value < 9; ++value)
            for (int scan = 0; scan < 9; ++scan)
            {
                // update row frequencies
                int rowIndex = row + scan;
                if (!metaState->set[rowIndex] && metaState->possibles[rowIndex][value])
                {
                    rPos[value] = rowIndex;
                    rFreq[value]++;
                }
                // update sub frequencies
                int colIndex = col + (scan * 9);
                if (!metaState->set[colIndex] && metaState->possibles[colIndex][value])
                {
                    cPos[value] = colIndex;
                    cFreq[value]++;
                }
                // update sub frequencies
                int subIndex = subgrid2index[index][scan];
                if (!metaState->set[subIndex] && metaState->possibles[subIndex][value])
                {
                    sPos[value] = subIndex;
                    sFreq[value]++;
                }
            }

        // set singles
        for (int value = 0; value < 9; ++value)
        {
            if (rFreq[value] == 1)
            {
                game->setCell(rPos[value], value);
                metaState->setCell(rPos[value], value);
            }
            if (cFreq[value] == 1)
            {
                game->setCell(cPos[value], value);
                metaState->setCell(cPos[value], value);
            }
            if (sFreq[value] == 1)
            {
                game->setCell(sPos[value], value);
                metaState->setCell(sPos[value], value);
            }
        }
    };

public:
    Solver(Game *game) : game(game)
    {
        states.push(pair<Board, MetaState>{*game->getBoard(), MetaState()});
    }

    int run()
    {
        Board *board = &states.top().first;
        MetaState *metaState = &states.top().second;

        for (int index = 0; index < 81; ++index)
        {
            char ch = board->readCell(index);
            if (ch != '-')
                metaState->setCell(index, ch - '1');
        }

        int iterations = -1;
        while (metaState->incomplete() && iterations < 20)
        {
            iterations++;
            // single possiblity setting
            for (int index = 0; index < 81; ++index)
                if (!metaState->set[index] && metaState->getPossibleSize(index) == 1)
                {
                    int value = metaState->getSingle(index);
                    game->setCell(index, value);
                    metaState->setCell(index, value);
                }

            for (int sweeper = 0; sweeper < 9; ++sweeper)
                frequencyAnalysis(sweeper, metaState);
        }

        metaState->listIncomplete();

        return iterations;
    }
};
