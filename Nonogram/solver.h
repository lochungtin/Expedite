#include <regex>
#include <utility>
#include <vector>

#include "game.h"
#include "../binaryboard.h"

using std::pair;
using std::regex;
using std::regex_match;
using std::sregex_iterator;
using std::to_string;
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

/**
 * @brief Constraint struct to maintain individual constraint data
 */
struct Constraint
{
    Constraint(int line, int length, int start, int end) : line(line), length(length), start(start), end(end){};
    int line;              // line index
    int length;            // length of constraint
    int start;             // minimum starting index
    int end;               // maximum ending index
    bool complete = false; // satisfaction status
};

/**
 * @brief Section struct to store subsections split data of lines
 */
struct Section
{
    Section(int start, int length, int fPad) : start(start), length(length), fPad(fPad){};
    int start;    // starting position
    int length;   // length of section
    int fPad;     // front padding
    int bPad = 0; // back padding
};

class Solver
{
private:
    // ===== variables =====
    Game *game;
    BinaryBoard *board;

    // dimension variables
    int dim;
    int lines;

    // constraint variables
    vector<vector<Constraint>> constraints;
    vector<regex> regexes = vector<regex>();

    // completion variables
    vector<bool> completion;

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
        return Constraint(lineIndex, constraints->at(index), frontSize, backSize);
    }

    /**
     * @brief Create a regex for the line of constraints
     *
     * @param constraints   line constraints
     * @param vectorSize    size of line constraints
     * @return corresponding regex
     */
    regex createConstraintRegex(vector<int> *constraints, int vectorSize)
    {
        // start with any number of 0s
        string builder = "[0-]*";

        // iterate over the constraints integer values
        for (int i = 0; i < vectorSize; ++i)
        {
            // add 1{n} for n number of 1s
            builder += "1{" + to_string(constraints->at(i)) + "}";

            // if not the last constraint, add 0+ for 1 or more 0s tailing the constraint
            if (i < vectorSize - 1)
                builder += "[0-]+";
        }

        // add final 0* for any number of 0s tailing the line
        return regex(builder + "[0-]*");
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

    //============================================================================
    // TOGGLE POINTER VALUES OVERLAP
    //============================================================================
    void markCertain0(vector<Constraint> *constraints, int line)
    {
        vector<int> togglePoints = vector<int>();
        for (int i = 0; i < constraints->size(); ++i)
        {
            togglePoints.emplace_back(constraints->at(i).start);
            togglePoints.emplace_back(constraints->at(i).end);
        }

        bool markerDown = true;
        int togglePointer = 0;

        for (int i = 0; i < dim; ++i)
        {
            if (i == togglePoints[togglePointer])
            {
                markerDown = !markerDown;
                togglePointer++;
            }

            if (markerDown)
                board->setCellByLine(line, i, false);
        }
    }

    /**
     * @brief Check if line completely satisfies constraints
     *
     * @param line      line string
     * @param lineIndex line index
     * @return true     - line constraints satisfied
     * @return false    - line constraints not yet complete
     */
    bool checkComplete(string line, int lineIndex)
    {
        // skip checks if already complete
        if (completion[lineIndex])
            return true;

        // check if line is complete, return false is not complete
        if (!regex_match(line, regexes[lineIndex]))
            return false;

        // fill all remaining cells with 0
        for (int i = 0; i < dim; ++i)
            board->setCellByLine(lineIndex, i, false);

        return true;
    }

    /**
     * @brief Analyse given line and split sections into marked and empty
     *
     * @param line      line to analyse
     * @param marked    marked array
     * @param empty     empty array
     */
    vector<Section> splitLineToSections(string line)
    {
        vector<Section> sections;

        int lengthCounter = 0;
        int emptyCounter = 0;
        int pointer = 0;

        for (int i = 0; i < dim; ++i)
        {
            if (line[i] == '0' && emptyCounter > 0)
            {
                sections[pointer++].bPad = emptyCounter;
                emptyCounter = 0;
            }
            else if (line[i] == '-')
            {
                emptyCounter++;

                if (lengthCounter > 0)
                {
                    marked->emplace_back(Section(dim - lengthCounter, lengthCounter));
                }
            }
        }

        // add last section if loop exited with counters > 0
        if (lengthCounter > 0)
            marked->emplace_back(Section(dim - lengthCounter, lengthCounter));
    }

public:
    Solver(Game *game) : game(game)
    {
        // set board pointer
        board = game->getBoard();

        // store dimension
        dim = board->getDim();
        lines = dim * 2;

        // initialise constraint array
        constraints = vector<vector<Constraint>>(lines, vector<Constraint>());

        // initialise completion array
        completion = vector<bool>(lines, false);

        // ===== INITIAL PREPARATION CODE =====
        // analyse constraints, map int to Constraint struct, create regex checker for constraint
        vector<vector<int>> *rows = game->getRowConstrinats();
        for (int i = 0; i < rows->size(); ++i)
        {
            vector<int> *rowCon = &rows->at(i);
            int rowLength = rowCon->size();

            // create line regex
            regexes.emplace_back(createConstraintRegex(rowCon, rowLength));

            // create individual constraint structs
            for (int j = 0; j < rowLength; ++j)
                constraints[i].emplace_back(createConstraintStruct(rowCon, rowLength, j, i));
        }

        vector<vector<int>> *cols = game->getColConstrinats();
        for (int i = 0; i < cols->size(); ++i)
        {
            vector<int> *colCon = &cols->at(i);
            int colLength = colCon->size();

            // create line regex
            regexes.emplace_back(createConstraintRegex(colCon, colLength));

            // create individual constraint structs
            int lineIndex = dim + i;
            for (int j = 0; j < colLength; ++j)
                constraints[lineIndex].emplace_back(createConstraintStruct(colCon, colLength, j, lineIndex));
        }
    }

    /**
     * @brief Run solver on board
     *
     * @return iterations taken to solve
     */
    int run()
    {
        int iterations = 0;

        for (; iterations < 1; ++iterations)
        {
            // mark certain cells
            for (int i = 0; i < lines; ++i)
            {
                if (completion[i])
                    continue;

                // mark all cells that are certain to have value 1
                for (auto constraint : constraints[i])
                    constraint.complete = markCertain1(&constraint);
            }

            // update line completion status
            for (int i = 0; i < lines; ++i)
            {
                string line = board->read(i);

                // check for line completion
                if (checkComplete(line, i))
                {
                    completion[i] = true;
                    continue;
                }
            }

            // reduce possible range of constraints
            for (int i = 0; i < lines; ++i)
            {
                // skip reduction step if line is already complete
                if (completion[i])
                    continue;

                string line = board->read(i);
                vector<Constraint> *lineConstraints = &constraints[i];

                // analyse line by spliting marked cells into sections
                vector<Section> marked = splitLineToSections(line);

                int sectionSize = marked.size();
                int constraintSize = lineConstraints->size();

                // number of sections is EQUAL TO number of constraints
                if (sectionSize == constraintSize)
                {
                    // only 1 section present and only 1 constraint required
                    if (sectionSize == 1)
                    {
                        // update constraint possible range
                        int cLength = lineConstraints->at(0).length;
                        lineConstraints->at(0).start = marked[0].start + marked[0].length - cLength;
                        lineConstraints->at(0).end = marked[0].start + cLength;
                        markCertain0(lineConstraints, i);
                    }
                    else
                    {
                        for (int j = 0; j < constraintSize; ++j)
                        {
                            cout << i << ": " << lineConstraints->at(j).start << " " << lineConstraints->at(j).end << " " << lineConstraints->at(j).length << endl;
                        }

                        for (int j = 0; j < sectionSize; ++j)
                        {
                            cout << i << ": " << marked[j].start << " " << marked[j].length << endl;
                        }
                    }
                }
                // number of sections is MORE THAN number of constraints
                else if (sectionSize > constraintSize)
                {
                }
                // number of sections is LESS THAN number of constraints
                else
                {
                }

                // mark all cells that are certain with value 0
            }
        }

        return iterations;
    }
};
