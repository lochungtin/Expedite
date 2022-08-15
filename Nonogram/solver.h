#include <regex>
#include <utility>
#include <vector>

#include "game.h"
#include "../binaryboard.h"

using std::pair;
using std::regex;
using std::sregex_iterator;
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

/**
 * @brief Constraint struct to maintain individual constraint data
 */
struct Constraint
{
    int line;              // line index
    int length;            // length of constraint
    int start;             // minimum starting index
    int end;               // maximum ending index
    bool complete = false; // satisfaction status
};

class Solver
{
private:
    // ===== variables =====
    Game *game;
    BinaryBoard *board;

    int dim;
    vector<vector<Constraint>> constraints;
    vector<regex> regexes;

    // ===== auxiliary functions =====
    /**
     * @brief Create a Constraint struct from given line constrains
     *
     * @param constraints   line constraints
     * @param index         index of target constraint
     * @param vectorSize    size of line constraints
     * @return Constraint struct
     */
    Constraint createConstraintStruct(vector<int> *constraints, int vectorSize, int index, int lineIndex)
    {
        // set start and end with max values
        int frontSize = 0;
        int backSize = dim;

        // add sizes of preceeding constraints to front
        for (int i = 0; i < index; ++i)
            frontSize += constraints->at(i) + 1;

        // add sizes of tailing constraints to back
        for (int i = index + 1; i < vectorSize; ++i)
            backSize -= constraints->at(i) + 1;

        // create Constraint struct
        Constraint c = Constraint();
        c.line = lineIndex;
        c.length = constraints->at(index);
        c.start = frontSize;
        c.end = backSize;

        return c;
    }

    /**
     * @brief Mark all cells that are the certain to have the value 1 in the given constraint
     *
     * @param constraint constraint to be checked
     * @return true     - constraint satisfied
     * @return false    - constraint not yet satisfied
     */
    bool markCertain1(Constraint *constraint)
    {
        // check if there are certain cells to be marked
        int availableLength = constraint->end - constraint->start;
        int half = (availableLength + 1) / 2;
        if (constraint->length < half)
            return false;

        // calculate starting position of certain cells
        int start = availableLength - constraint->length;

        // mark all certain cells
        for (int i = start; i < constraint->length; ++i)
            board->setCellByLine(constraint->line, i + constraint->start, true);

        // return true if constraint is satisfied
        return availableLength == constraint->length;
    }

    bool checkComplete()

        public : Solver(Game *game) : game(game)
    {
        // set board pointer
        board = game->getBoard();

        // store dimension
        dim = board->getDim();

        constraints = vector<vector<Constraint>>(dim * 2, vector<Constraint>());
    }

    /**
     * @brief Run solver on board
     *
     * @return iterations taken to solve
     */
    int run()
    {
        // analyse constraints, map int to Constraint struct, create regex checker for constraint
        vector<vector<int>> *rows = game->getRowConstrinats();
        for (int i = 0; i < rows->size(); ++i)
        {
            vector<int> *rowCon = &rows->at(i);
            int rowLength = rowCon->size();

            for (int j = 0; j < rowLength; ++j)
                constraints[i].emplace_back(createConstraintStruct(rowCon, rowLength, j, i));
        }

        vector<vector<int>> *cols = game->getColConstrinats();
        for (int i = 0; i < cols->size(); ++i)
        {
            int lineIndex = dim + i;

            vector<int> *colCon = &cols->at(i);
            int colLength = colCon->size();

            for (int j = 0; j < colLength; ++j)
                constraints[lineIndex].emplace_back(createConstraintStruct(colCon, colLength, j, lineIndex));
        }

        int iterations = 0;

        for (auto lineConstraint : constraints)
            for (auto constraint : lineConstraint)
                constraint.complete = markCertain(&constraint);

        return iterations;
    }
};
