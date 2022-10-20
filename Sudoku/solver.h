#include <map>
#include <stack>
#include <utility>

#include "game.h"
#include "metastate.h"
#include "../board.h"

using std::pair;
using std::stack;

struct FrequencyTable
{
    int singleFrequency[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int singleLocation[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    map<int, int> doublePairFrequency;
    map<int, int> doublePairLocation;
};

struct SweepAnalysis
{
    FrequencyTable rFT;
    FrequencyTable cFT;
    FrequencyTable sFT;
};

class Solver
{
private:
    // ===== variables =====
    Game *game;
    stack<pair<Board, MetaState>> states;

    void updateBoards(MetaState *metaState, int index, int value)
    {
        game->setCell(index, value);
        metaState->setCell(index, value);
    }

    SweepAnalysis frequencyAnalysis(int index, MetaState *metaState)
    {
        // frequency table
        SweepAnalysis sa;

        // absolute values
        int row = (index / 9) * 9;
        int col = index % 9;

        // scanning
        for (int scan = 0; scan < 9; ++scan)
        {
            int rowIndex = row + scan;
            int colIndex = col + (scan * 9);
            int subIndex = subgrid2index[index][scan];
            for (int value = 0; value < 9; ++value)
            {
                // update row single frequencies
                if (!metaState->set[rowIndex] && metaState->possibles[rowIndex][value])
                {
                    sa.rFT.singleLocation[value] = rowIndex;
                    sa.rFT.singleFrequency[value]++;
                }
                // update col single frequencies
                if (!metaState->set[colIndex] && metaState->possibles[colIndex][value])
                {
                    sa.cFT.singleLocation[value] = colIndex;
                    sa.cFT.singleFrequency[value]++;
                }
                // update sub single frequencies
                if (!metaState->set[subIndex] && metaState->possibles[subIndex][value])
                {
                    sa.sFT.singleLocation[value] = subIndex;
                    sa.sFT.singleFrequency[value]++;
                }
            }

            // update row double pair frequencies
            if (metaState->getPossibleSize(rowIndex) == 2)
            {

            }
        }

        return sa;
    };

    int possibles2signature(bool *possibles)
    {
        int shift = 1;
        int signature = 0;
        for (int i = 0; i < 9; ++i)
        {
            if (possibles[i])
            {
                signature += i * shift;
                shift *= 10;
            }
        }

        return signature;
    }

    // 3150

public:
    Solver(Game *game) : game(game)
    {
        states.push(pair<Board, MetaState>{*game->getBoard(), MetaState()});
    }

    int run()
    {
        Board *board = &states.top().first;
        MetaState *metaState = &states.top().second;

        // create meta state from board input
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
                    updateBoards(metaState, index, metaState->getSingle(index));

            for (int sweeper = 0; sweeper < 9; ++sweeper)
                SweepAnalysis sa = frequencyAnalysis(sweeper, metaState);
        }

        metaState->listIncomplete();

        return iterations;
    }
};
