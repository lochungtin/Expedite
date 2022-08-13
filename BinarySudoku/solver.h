#include <regex>
#include <utility>
#include <vector>

#include "game.h"
#include "../headers/binaryboard.h"

using std::pair;
using std::regex;
using std::sregex_iterator;
using std::vector;

#define PATTERN_REGEX_PAIRS \
    vector<pair<regex, int>> { pair<regex, int>(regex("-11"), 0), pair<regex, int>(regex("-00"), 0), pair<regex, int>(regex("11-"), 2), pair<regex, int>(regex("00-"), 2), pair<regex, int>(regex("1-1"), 1), pair<regex, int>(regex("0-0"), 1) }

class Solver
{
private:
    Game *game;
    BinaryBoard *board;

    int dim;
    int hDim;
    int dDim;
    int cellCount;

    vector<vector<int>> counts;

    bool boardComplete = false;
    vector<bool> completion;

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
        counts[cLine][0] += isOne * 1;
        counts[cLine][0] += !isOne * 1;

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

public:
    Solver(Game *game) : game(game)
    {
        board = game->getBoard();

        dim = board->getDim();
        hDim = dim / 2;
        dDim = dim * 2;
        cellCount = dim * dim;

        counts = vector<vector<int>>();
        completion = vector<bool>(dDim, false);

        for (int i = 0; i < dDim; ++i)
            counts.emplace_back(vector<int>(2, 0));
    }

    void run()
    {
        for (int a = 0; a < 2; ++a)
        {
            // enforce rules for each line
            for (int line = 0; line < dDim; ++line)
            {
                // read current line
                string lRead = board->read(line);

                // enforce pattern base rules
                if (!completion[line])
                    for (int actionIndex = 0; actionIndex < 6; ++actionIndex)
                    {
                        pair<regex, int> pattern = PATTERN_REGEX_PAIRS[actionIndex];
                        vector<int> positions;
                        sregex_iterator itr = sregex_iterator(lRead.begin(), lRead.end(), pattern.first);
                        for (; itr != sregex_iterator(); ++itr)
                            setCell(line2index(line, (*itr).position(0) + pattern.second), actionIndex % 2);

                        if (completion[line])
                            break;
                    }

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
            }
        }
    }
};
