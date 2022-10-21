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

    int doubleFrequency[36] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int doubleLocation[36] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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

    /**
     * @brief Update metaState and game board
     *
     * @param metaState metaState pointer
     * @param index     board index
     * @param value     value to set
     */
    void updateBoard(MetaState *metaState, int index, int value)
    {
        game->setCell(index, value);
        metaState->setCell(index, value);
    }

    /**
     * @brief Perform frequency analysis on possible values on the metaState
     *
     * @param metaState metaState pointer
     * @param index     sweeper index
     * @return SweepAnalysis (tuple of frequency tables)
     */
    void frequencyAnalysis(SweepAnalysis *sa, MetaState *metaState, int sweeper)
    {
        // loop through all relative indices w.r.t. sweeper index
        for (int scan = 0; scan < 9; ++scan)
        {
            // calculate sweep indices
            int rowIndex = sweeper * 9 + scan;
            int colIndex = sweeper + (scan * 9);
            int subIndex = subgrid2index(sweeper, scan);

            // update
            for (int value = 0; value < 9; ++value)
            {
                if (!metaState->set[rowIndex] && metaState->possibles[rowIndex][value])
                    singleFrequencyUpdate(&sa->rFT, rowIndex, value);
                if (!metaState->set[colIndex] && metaState->possibles[colIndex][value])
                    singleFrequencyUpdate(&sa->cFT, colIndex, value);
                if (!metaState->set[subIndex] && metaState->possibles[subIndex][value])
                    singleFrequencyUpdate(&sa->sFT, subIndex, value);
            }

            // update row double pair frequencies
            if (!metaState->set[rowIndex] && metaState->getPossibleSize(rowIndex) == 2)
            {
                // cout << scan << " r " << rowIndex << endl;
                doubleFrequencyUpdate(metaState, &sa->rFT, rowIndex, scan);
            }

            if (!metaState->set[colIndex] && metaState->getPossibleSize(colIndex) == 2)
            {
                // cout << scan << " c " << colIndex << endl;
                doubleFrequencyUpdate(metaState, &sa->cFT, colIndex, scan);
            }

            if (!metaState->set[subIndex] && metaState->getPossibleSize(subIndex) == 2)
            {
                // cout << scan << " s " << subIndex << endl;
                doubleFrequencyUpdate(metaState, &sa->sFT, subIndex, scan);
            }
        }
    };

    /**
     * @brief Update frequency table with single possible data
     *
     * @param ft    frequency table pointer
     * @param value possible value to update
     * @param index location of possible value
     */
    void singleFrequencyUpdate(FrequencyTable *ft, int index, int value)
    {
        ft->singleLocation[value] = index;
        ft->singleFrequency[value]++;
    }

    /**
     * @brief Update frequency table with double possible value data
     *
     * @param metaState metaState pointer
     * @param ft        frequency table pointer
     * @param index     board index
     * @param scan      relative index
     */
    void doubleFrequencyUpdate(MetaState *metaState, FrequencyTable *ft, int index, int scan)
    {
        int pair = 0;
        int shift = 10;
        for (int i = 0; i < 9; ++i)
            if (metaState->possibles[index][i])
            {
                pair += i * shift;
                shift /= 10;
            }

        int sign = pair2signature(pair);

        ft->doubleLocation[sign] += (10 - 9 * ft->doubleFrequency[sign]) * scan;
        ft->doubleFrequency[sign]++;
    }

public:
    Solver(Game *game) : game(game)
    {
        states.push(pair<Board, MetaState>{*game->getBoard(), MetaState()});
    }

    /**
     * @brief Run solver on board
     *
     * @return iterations taken to solve
     */
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

            // frequency analysis for possible value pruning
            for (int sweeper = 0; sweeper < 9; ++sweeper)
            {
                SweepAnalysis sa;
                frequencyAnalysis(&sa, metaState, sweeper);

                // prune possible values from cells with double pair rule
                for (int sign = 0; sign < 36; ++sign)
                {
                    if (sa.rFT.doubleFrequency[sign] == 2)
                    {
                        int location = sa.rFT.doubleLocation[sign];
                        int l0 = location / 10;
                        int l1 = location % 10;

                        int pair = signature2pair(sign);
                        int p0 = pair / 10;
                        int p1 = pair % 10;

                        for (int index = 0; index < 9; ++index)
                        {
                            int rowIndex = sweeper * 9 + index;
                            if (!metaState->set[rowIndex] && index != l0 && index != l1)
                            {
                                metaState->possibles[rowIndex][p0] = false;
                                metaState->possibles[rowIndex][p1] = false;
                            }
                        }
                    }

                    if (sa.cFT.doubleFrequency[sign] == 2)
                    {
                        int location = sa.cFT.doubleLocation[sign];
                        int l0 = location / 10;
                        int l1 = location % 10;

                        int pair = signature2pair(sign);
                        int p0 = pair / 10;
                        int p1 = pair % 10;

                        for (int index = 0; index < 9; ++index)
                        {
                            int colIndex = sweeper + index * 9;
                            if (!metaState->set[colIndex] && index != l0 && index != l1)
                            {
                                metaState->possibles[colIndex][p0] = false;
                                metaState->possibles[colIndex][p1] = false;
                            }
                        }
                    }

                    if (sa.sFT.doubleFrequency[sign] == 2)
                    {
                        int location = sa.sFT.doubleLocation[sign];
                        int l0 = location / 10;
                        int l1 = location % 10;

                        int pair = signature2pair(sign);
                        int p0 = pair / 10;
                        int p1 = pair % 10;

                        for (int index = 0; index < 9; ++index)
                        {
                            int subIndex = subgrid2index(sweeper, index);
                            if (!metaState->set[subIndex] && index != l0 && index != l1)
                            {
                                metaState->possibles[subIndex][p0] = false;
                                metaState->possibles[subIndex][p1] = false;
                            }
                        }
                    }
                }

                // set cells to value with freq 1
                for (int value = 0; value < 9; ++value)
                {
                    if (sa.rFT.singleFrequency[value] == 1)
                        updateBoard(metaState, sa.rFT.singleLocation[value], value);
                    if (sa.cFT.singleFrequency[value] == 1)
                        updateBoard(metaState, sa.cFT.singleLocation[value], value);
                    if (sa.sFT.singleFrequency[value] == 1)
                        updateBoard(metaState, sa.sFT.singleLocation[value], value);
                }
            }

            // set cells with only one possible value left
            for (int index = 0; index < 81; ++index)
                if (!metaState->set[index] && metaState->getPossibleSize(index) == 1)
                    updateBoard(metaState, index, metaState->getSingle(index));
        }

        // metaState->listIncomplete();

        return iterations;
    }
};
