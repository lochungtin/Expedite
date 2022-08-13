#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "board.h"

using std::cout;
using std::endl;
using std::pair;
using std::regex;
using std::regex_match;
using std::string;
using std::to_string;
using std::vector;

struct Constraint
{
    int line;
    int index;
    int length;
    // range [rMin, rMax)
    int rMin = 0;
    int rMax = 0;
    bool complete = false;
};

struct Section
{
    int start;
    int length = 0;
    int link;
};

class Game
{
private:
    // dimensions
    int dimSize;
    int lines;

    // data
    Board board;

    // constraints
    int maxConstraintNum = 0;

    vector<int> constraintSum;
    vector<vector<Constraint>> constraintsVec;
    vector<regex> constraintsRegexLoose;
    vector<regex> constraintsRegexAbs;

    // completion
    vector<bool> completion;

    // ===== auxiliary functions =====
    // get minimum space required for constraints
    int getMinSpace(vector<Constraint> *constraints, int start, int end)
    {
        int sum = 0;
        for (int i = start; i < end; ++i)
            sum += (constraints->at(i).length + 1);

        return sum;
    }

    // fill in certain cells by constraint range
    void fillCertain(int line)
    {
        int size = constraintsVec[line].size();
        vector<Constraint> *constraint = &constraintsVec[line];

        for (int index = 0; index < size; ++index)
        {
            Constraint c = constraint->at(index);
            int start = c.rMax - c.length;
            int end = c.rMin + c.length;

            for (int i = start; i < end; ++i)
                board.setCellByLine(line, i, true);

            if (end - start == c.length)
                c.complete = true;
        }
    }

    // analyse line partitions, get marked and unmarked sections
    void analysePartitions(string target, vector<Section> *marked, vector<Section> *empty)
    {
        Section markedSection;
        Section emptySection;

        int emptyCount = 0;

        for (int index = 0; index < dimSize; ++index)
        {
            char ch = target[index];

            if (ch == '1')
            {
                if (markedSection.length == 0)
                    markedSection.start = index;
                markedSection.length++;
            }
            else if (markedSection.length > 0)
            {
                markedSection.link = emptyCount;
                marked->emplace_back(markedSection);
                markedSection = Section();
            }

            if (ch != '0')
            {
                if (emptySection.length == 0)
                    emptySection.start = index;
                emptySection.length++;
            }
            else if (emptySection.length > 0)
            {
                empty->emplace_back(emptySection);
                emptySection = Section();
                emptyCount++;
            }
        }
        if (markedSection.length > 0)
            marked->emplace_back(markedSection);

        if (emptySection.length > 0)
            empty->emplace_back(emptySection);
    }

    // check if line is complete
    bool isComplete(int line, bool checkingOverride = false, bool requireFill = true)
    {
        if (completion[line])
            return true;

        string target = board.read(line);
        if (checkingOverride || regex_match(target, constraintsRegexAbs[line]))
        {
            if (requireFill)
                for (int i = 0; i < dimSize; ++i)
                    board.setCellByLine(line, i, false);

            completion[line] = true;
            return true;
        }

        return false;
    }

public:
    Game() {}
    Game(int dimIn)
    {
        // initialise board
        dimSize = dimIn;
        lines = dimIn * 2;
        board = Board(dimSize);

        // initialise constraints data structures
        constraintSum = vector<int>(lines, 0);

        constraintsVec = vector<vector<Constraint>>(lines, vector<Constraint>());

        regex DEF("x");
        constraintsRegexLoose = vector<regex>(lines, DEF);
        constraintsRegexAbs = vector<regex>(lines, DEF);

        // initialise completion maintainers
        completion = vector<bool>(lines, false);
    }

    // set constraints
    void setRowConstraints(int index, vector<int> constraints)
    {
        int size = constraints.size();

        vector<Constraint> lineConstraints = vector<Constraint>(size, Constraint());

        string abs = "^[0|-]*";
        string loose = "^[0|-]*";

        for (int i = 0; i < size; ++i)
        {
            int len = constraints[i];

            // create constraint struct
            lineConstraints[i].line = index;
            lineConstraints[i].index = i;
            lineConstraints[i].length = len;

            // create matching regex
            constraintSum[index] += len;
            abs += "1{" + to_string(len) + (i + 1 < size ? "}[0|-]+" : "}");
            loose += "[1|";
            loose += (size > 1 ? "0|" : "");
            loose += "-]{" + to_string(len) + (i + 1 < size ? "}[0|-]+" : "}");
        }

        constraintsRegexAbs[index] = regex(abs + "[0|-]*$");
        constraintsRegexLoose[index] = regex(loose + "[0|-]*$");

        // maintain max constraint number
        if (size > maxConstraintNum)
            maxConstraintNum = size;

        // maintain constraint vector
        constraintsVec[index] = lineConstraints;
    }

    void setColConstraints(int col, vector<int> constraints)
    {
        setRowConstraints(col + dimSize, constraints);
    }

    // solve board
    void solve()
    {
        // maintain initial constraint range
        for (int line = 0; line < lines; ++line)
        {
            int size = constraintsVec[line].size();
            vector<Constraint> *constraint = &constraintsVec[line];

            for (int index = 0; index < size; ++index)
            {
                constraint->at(index).rMin = (getMinSpace(constraint, 0, index));
                constraint->at(index).rMax = dimSize - getMinSpace(constraint, index + 1, size);
            }
        }

        // fill all cells are the certain
        for (int line = 0; line < lines; ++line)
            fillCertain(line);

        // mark completion of lines
        for (int line = 0; line < lines; ++line)
            isComplete(line);

        // rule pruning
        for (int line = 0; line < lines; ++line)
        {
            if (completion[line])
                continue;

            string target = board.read(line);

            vector<Section> markedSections;
            vector<Section> emptySections;
            analysePartitions(target, &markedSections, &emptySections);

            int markedCount = markedSections.size();
            int emptyCount = emptySections.size();

            int constraintCount = constraintsVec[line].size();

            if (markedCount == constraintCount)
                for (int markedId = 0; markedId < markedCount; ++markedId)
                {
                    Section marked = markedSections[markedId];
                    int required = constraintsVec[line][markedId].length;
                    int exMin = marked.start + marked.length - required;
                    int exMax = marked.start + required;

                    Section empty = emptySections[marked.link];
                    int eEnd = empty.start + empty.length;
                    // if (eEnd - exMin > 0)
                }
            else if (markedCount < constraintCount)
            {
            }
            else
            {
            }
        }
    }

    // ===== getters for auxiliary helper functions =====
    string getBoard()
    {
        return board.getBoardAsString();
    }

    int getDim()
    {
        return dimSize;
    }

    int getMaxConstraintNum()
    {
        return maxConstraintNum;
    }

    vector<vector<Constraint>> getConstraints()
    {
        return constraintsVec;
    }
};

#endif
