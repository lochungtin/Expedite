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

    int rSpan[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
    int cSpan[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
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
     * @return true
     */
    bool updateBoard(MetaState *metaState, int index, int value)
    {
        game->setCell(index, value);
        metaState->setCell(index, value);

        return true;
    }

    /**
     * @brief Perform frequency analysis on possible values on the metaState
     *
     * @param sa        sweep analysis struct pointer
     * @param metaState metaState pointer
     * @param index     sweeper index
     */
    void frequencyAnalysis(SweepAnalysis *sa, MetaState *metaState, int sweeper)
    {
        bool rSpanRegisters[3][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0}};
        bool cSpanRegisters[3][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0}};

        // loop through all relative indices w.r.t. sweeper index
        for (int scan = 0; scan < 9; ++scan)
        {
            // calculate sweep indices
            int rowIndex = sweeper * 9 + scan;
            int colIndex = sweeper + (scan * 9);
            int subIndex = subgrid2index(sweeper, scan);

            int subRow = scan / 3;
            int subCol = scan % 3;

            // update row frequency data
            if (!metaState->set[rowIndex])
            {
                for (int value = 0; value < 9; ++value)
                    if (metaState->possibles[rowIndex][value])
                        singleFrequencyUpdate(&sa->rFT, rowIndex, value);
                if (metaState->getPossibleSize(rowIndex) == 2)
                    doubleFrequencyUpdate(metaState, &sa->rFT, rowIndex, scan);
            }
            // update column frequency data
            if (!metaState->set[colIndex])
            {
                for (int value = 0; value < 9; ++value)
                    if (metaState->possibles[colIndex][value])
                        singleFrequencyUpdate(&sa->cFT, colIndex, value);
                if (metaState->getPossibleSize(colIndex) == 2)
                    doubleFrequencyUpdate(metaState, &sa->cFT, colIndex, scan);
            }
            // update subgrid frequency data
            if (!metaState->set[subIndex])
            {
                for (int value = 0; value < 9; ++value)
                    if (metaState->possibles[subIndex][value])
                    {
                        singleFrequencyUpdate(&sa->sFT, subIndex, value);

                        rSpanRegisters[subRow][value] = true;
                        cSpanRegisters[subCol][value] = true;
                    }

                if (metaState->getPossibleSize(subIndex) == 2)
                    doubleFrequencyUpdate(metaState, &sa->sFT, subIndex, scan);
            }
        }

        // update sub grid span data
        for (int value = 0; value < 9; ++value)
            for (int sweep = 0; sweep < 3; ++sweep)
            {
                if (rSpanRegisters[sweep][value] && sa->rSpan[value] != 5)
                {
                    if (sa->rSpan[value] != 4)
                        sa->rSpan[value] = 5;
                    else
                        sa->rSpan[value] = sweep;
                }
                if (cSpanRegisters[sweep][value] && sa->cSpan[value] != 5)
                {
                    if (sa->cSpan[value] != 4)
                        sa->cSpan[value] = 5;
                    else
                        sa->cSpan[value] = sweep;
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

    /**
     * @brief Remove possible values given double pair
     *
     * @param metaState metaState pointer
     * @param index     board index
     * @param p0        pair[0]
     * @param p1        pair[1]
     */
    void removePossiblePair(MetaState *metaState, int index, int p0, int p1)
    {
        metaState->possibles[index][p0] = false;
        metaState->possibles[index][p1] = false;
    }

public:
    Solver(Game *game) : game(game)
    {
        // initial metaState
        MetaState metaState = MetaState();

        // add initial cells to metaState
        for (int index = 0; index < 81; ++index)
        {
            char ch = game->getBoard()->readCell(index);
            if (ch != '-')
                metaState.setCell(index, ch - '1');
        }

        // push board metaState pair to state stack
        states.push(pair<Board, MetaState>{*game->getBoard(), metaState});
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

        bool changed = true;
        int iterations = -1;
        while (metaState->incomplete() && changed)
        {
            changed = false;
            iterations++;

            // frequency analysis for possible value pruning
            for (int sweeper = 0; sweeper < 9; ++sweeper)
            {
                SweepAnalysis sa;
                frequencyAnalysis(&sa, metaState, sweeper);

                // prune possible values from cells with subgrid single span rule
                int rowOffset = (sweeper / 3) * 3;
                int rowBound = ((sweeper / 3) + 1) * 3;
                int colOffset = (sweeper % 3) * 3;
                int colBound = ((sweeper % 3) + 1) * 3;
                for (int value = 0; value < 9; ++value)
                {
                    if (sa.rSpan[value] < 3)
                    {
                        int targetRow = sa.rSpan[value] + rowOffset;
                        for (int col = 0; col < 9; ++col)
                            if (col < colOffset || col >= colBound)
                                metaState->possibles[targetRow * 9 + col][value] = false;
                    }
                    if (sa.cSpan[value] < 3)
                    {
                        int targetCol = sa.cSpan[value] + colOffset;
                        for (int row = 0; row < 9; ++row)
                            if (row < rowOffset || row >= rowBound)
                                metaState->possibles[row * 9 + targetCol][value] = false;
                    }
                }

                // prune possible values from cells with double pair rule
                for (int sign = 0; sign < 36; ++sign)
                {
                    int pair = signature2pair(sign);
                    int p0 = pair / 10;
                    int p1 = pair % 10;

                    if (sa.rFT.doubleFrequency[sign] == 2)
                    {
                        int l0 = sa.rFT.doubleLocation[sign] / 10;
                        int l1 = sa.rFT.doubleLocation[sign] % 10;
                        for (int index = 0; index < 9; ++index)
                        {
                            int rowIndex = sweeper * 9 + index;
                            if (!metaState->set[rowIndex] && index != l0 && index != l1)
                                removePossiblePair(metaState, rowIndex, p0, p1);
                        }
                    }
                    if (sa.cFT.doubleFrequency[sign] == 2)
                    {
                        int l0 = sa.cFT.doubleLocation[sign] / 10;
                        int l1 = sa.cFT.doubleLocation[sign] % 10;
                        for (int index = 0; index < 9; ++index)
                        {
                            int colIndex = sweeper + index * 9;
                            if (!metaState->set[colIndex] && index != l0 && index != l1)
                                removePossiblePair(metaState, colIndex, p0, p1);
                        }
                    }
                    if (sa.sFT.doubleFrequency[sign] == 2)
                    {
                        int l0 = sa.sFT.doubleLocation[sign] / 10;
                        int l1 = sa.sFT.doubleLocation[sign] % 10;
                        for (int index = 0; index < 9; ++index)
                        {
                            int subIndex = subgrid2index(sweeper, index);
                            if (!metaState->set[subIndex] && index != l0 && index != l1)
                                removePossiblePair(metaState, subIndex, p0, p1);
                        }
                    }
                }

                // set cells to value with freq 1
                for (int value = 0; value < 9; ++value)
                {
                    if (!metaState->set[sa.rFT.singleLocation[value]] && sa.rFT.singleFrequency[value] == 1)
                        changed = updateBoard(metaState, sa.rFT.singleLocation[value], value);
                    if (!metaState->set[sa.cFT.singleLocation[value]] && sa.cFT.singleFrequency[value] == 1)
                        changed = updateBoard(metaState, sa.cFT.singleLocation[value], value);
                    if (!metaState->set[sa.sFT.singleLocation[value]] && sa.sFT.singleFrequency[value] == 1)
                        changed = updateBoard(metaState, sa.sFT.singleLocation[value], value);
                }
            }

            // set cells with only one possible value left
            for (int index = 0; index < 81; ++index)
                if (!metaState->set[index] && metaState->getPossibleSize(index) == 1)
                    changed = updateBoard(metaState, index, metaState->getSingle(index));
        }
        return iterations;
    }
};
