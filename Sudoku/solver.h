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
            // calculate sweep indices
            int rowIndex = row + scan;
            int colIndex = col + (scan * 9);
            int subIndex = subgrid2index(index, scan);

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
            for (int sweeper = 0; sweeper < 9; ++sweeper)
            {
                SweepAnalysis sa = frequencyAnalysis(sweeper, metaState);

                // set cells to value with freq 1
                for (int value = 0; value < 9; ++value)
                {
                    if (sa.rFT.singleFrequency[value] == 1)
                        updateBoards(metaState, sa.rFT.singleLocation[value], value);
                    if (sa.cFT.singleFrequency[value] == 1)
                        updateBoards(metaState, sa.cFT.singleLocation[value], value);
                    if (sa.sFT.singleFrequency[value] == 1)
                        updateBoards(metaState, sa.sFT.singleLocation[value], value);
                }
            }

            // set cells with only one possible value left
            for (int index = 0; index < 81; ++index)
                if (!metaState->set[index] && metaState->getPossibleSize(index) == 1)
                    updateBoards(metaState, index, metaState->getSingle(index));
        }

        metaState->listIncomplete();

        return iterations;
    }
};
