#include <regex>
#include <utility>
#include <vector>

#include "game.h"
#include "../binaryboard.h"

using std::pair;
using std::regex;
using std::sregex_iterator;
using std::vector;

class Solver
{
private:
    // ===== variables =====
    Game *game;
    BinaryBoard *board;

    int dim;
    int hDim;
    int dDim;
    int cellCount;

    vector<vector<int>> counts;

    bool boardComplete = false;
    vector<bool> completion;

    vector<pair<regex, int>> patterns = vector<pair<regex, int>>{
        pair<regex, int>(regex("-11"), 0), // row double left 1
        pair<regex, int>(regex("-00"), 0), // row double left 0
        pair<regex, int>(regex("11-"), 2), // row double right 1
        pair<regex, int>(regex("00-"), 2), // row double right 0
        pair<regex, int>(regex("1-1"), 1), // row single gap 1
        pair<regex, int>(regex("0-0"), 1), // row single gap 0
    };

    // ===== auxiliary functions =====
    /**
     * @brief Set the cell value to board and maintain completion status for lines
     *
     * @param index absolute index of cell
     * @param isOne value to be set
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCell(int index, bool isOne)
    {
        // update board
        if (!board->setCellByIndex(index, isOne))
            return false;

        // maintain counter status
        int rLine = index / dim;
        int cLine = index % dim + dim;
        counts[rLine][0] += !isOne * 1;
        counts[rLine][1] += isOne * 1;
        counts[cLine][0] += !isOne * 1;
        counts[cLine][1] += isOne * 1;

        // maintain line completion status
        if (counts[rLine][0] + counts[rLine][1] == dim)
            completion[rLine] = true;

        if (counts[cLine][0] + counts[cLine][1] == dim)
            completion[cLine] = true;

        // check for board completion
        bool complete = true;
        for (bool lineComplete : completion)
            complete *= lineComplete;

        boardComplete = complete;

        // return set cell completed
        return true;
    }

    /**
     * @brief Convert line-index position to absolute index position
     *
     * @param line  line index
     * @param index position index
     * @return absolute index position
     */
    int line2index(int line, int index)
    {
        if (line > dim)
            return index * dim + (line % dim);

        return line * dim + index;
    }

    /**
     * @brief Convert normal indexing to rotated indexing
     *
     * @param index normal index value
     * @return rotated index value
     */
    int indexRotation(int index)
    {
        return index % dim * dim + (index / dim);
    }

    /**
     * @brief Enforce pattern based rules, fill in empty cells by regex pattern
     *
     * @param boardString   boardString
     * @param rotated       rotation of board (0 or 1)
     */
    void enforcePatternRules(string boardString, int rotated)
    {
        // iterate over all 6 patterns
        for (int actionIndex = 0; actionIndex < 6; ++actionIndex)
        {
            pair<regex, int> pattern = patterns[actionIndex];
            vector<int> positions;
            sregex_iterator itr = sregex_iterator(boardString.begin(), boardString.end() - 1, pattern.first);

            // iterate over all matching patterns
            for (; itr != sregex_iterator(); ++itr)
            {
                // forbid updates on edge matches
                int matchStart = (*itr).position(0);
                if (dim - matchStart % dim > 2)
                {
                    int index = matchStart + pattern.second;
                    if (rotated)
                        index = indexRotation(index);

                    setCell(index, actionIndex % 2);
                }
            }
        }
    }

public:
    Solver(Game *game) : game(game)
    {
        // set board pointer
        board = game->getBoard();

        // store dimension
        dim = board->getDim();
        hDim = dim / 2;
        dDim = dim * 2;
        cellCount = dim * dim;

        // initialise 0 1 counters
        counts = vector<vector<int>>();
        for (int i = 0; i < dDim; ++i)
            counts.emplace_back(vector<int>(2, 0));

        // initialise line completion array
        completion = vector<bool>(dDim, false);

        // ===== INITIAL PREPARATION CODE =====
        // update counts with preset values
        for (int line = 0; line < dDim; ++line)
        {
            string lRead = board->read(line);
            for (int j = 0; j < dim; ++j)
            {
                counts[line][0] += lRead[j] == '0' * 1;
                counts[line][1] += lRead[j] == '1' * 1;
            }
        }
    }

    /**
     * @brief Run solver on board
     *
     * @return iterations taken to solve
     */
    int run()
    {
        int iterations = -1;
        while (true)
        {
            iterations++;
            // handle different orientations
            for (int rotationBool = 0; rotationBool < 2; ++rotationBool)
            {
                string boardString = board->read();
                if (rotationBool)
                    boardString = board->readRotated();

                // rule enforcer
                enforcePatternRules(boardString, rotationBool);

                // exit when complete
                if (boardComplete)
                    return iterations;
            }

            // enforce rules for each line
            for (int line = 0; line < dDim; ++line)
            {
                // read current line
                string lRead = board->read(line);

                // enforce half filled rule
                if (!completion[line])
                {
                    // check of the line is half filled with 0s or 1s
                    int c0 = counts[line][0] == hDim;
                    int c1 = counts[line][1] == hDim;
                    // fill remaining with the other type of value of half is reached for one
                    if (c0 != c1)
                        for (int index = 0; index < dim; ++index)
                            setCell(line2index(line, index), c0);
                }

                // exit when complete
                if (boardComplete)
                    return iterations;

                // enforce duplication prevention rule
                if (!completion[line] && counts[line][0] == hDim - 1 && counts[line][1] == hDim - 1)
                {
                    // find target line
                    string target = "";
                    vector<int> empties = vector<int>();

                    int targetStartRange = (line >= dim) * dim;
                    for (int tLine = targetStartRange; tLine < targetStartRange + dim; ++tLine)
                    {
                        // skip line if not complete
                        if (!completion[tLine] && tLine != line)
                            continue;

                        string tRead = board->read(tLine);

                        // check if each set character of the two lines match
                        bool earlyExit = false;
                        for (int index = 0; index < dim; ++index)
                        {
                            if (lRead[index] == '-')
                                empties.emplace_back(index);
                            else if (lRead[index] != tRead[index])
                            {
                                earlyExit = true;
                                break;
                            }
                        }

                        // continue search if early exit
                        if (earlyExit)
                            continue;

                        // set target string and exit search
                        target = tRead;
                        break;
                    }

                    // target line found, update current line
                    if (target != "")
                        for (int emptyPos : empties)
                            setCell(line2index(line, emptyPos), target[emptyPos] == '0');
                }

                // exit when complete
                if (boardComplete)
                    return iterations;
            }
        }
    }
};
