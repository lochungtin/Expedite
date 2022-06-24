#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "board.h"
#include "utils.h"
#include "dimq.h"

using std::cout;
using std::endl;
using std::pair;
using std::regex;
using std::regex_match;
using std::string;
using std::to_string;
using std::vector;

class Game
{
private:
    // dimensions
    int dim;
    int dim2;

    // data
    Board board;

    // constraints
    int maxConstraintNum = 0;

    vector<int> constraintSum;
    vector<regex> constraintsRegexLoose;
    vector<regex> constraintsRegexAbs;
    vector<vector<int>> constraintsVec;
    vector<vector<int>> constraintsVecOg;

    // mutated lines
    DIMQ mutated;

    // completion
    vector<bool> completion;

    // ===== subroutine auxiliaries =====

    // get first positive edge cell
    pair<int, int> getFirstMarkedEdgePosition(string line)
    {
        int first = -1;
        int last = -1;

        for (int i = 0; i < dim; ++i)
        {
            if (line[i] == '-')
                break;
            if (line[i] == '1')
            {
                first = i;
                break;
            }
        }

        for (int i = dim - 1; i >= 0; --i)
        {
            if (line[i] == '-')
                break;
            if (line[i] == '1')
            {
                last = i;
                break;
            }
        }

        return pair<int, int>(first, last);
    }

    // calculate the minimum space required for a set of constraints
    int sumConstraintMinSpace(vector<int> *vector, int start = 0, int end = -1)
    {
        int size = vector->size();
        if (end == -1)
            end = size;

        int sum = 0;
        for (int i = start; i < end; ++i)
            sum += (vector->at(i) + 1);

        return sum - 1;
    }

    // get remaining continuous non-negative spaces
    vector<pair<int, int>> getAllContinuousSpace(string line)
    {
        vector<pair<int, int>> spaces;
        int size = 0;
        int pointer = 0;
        bool valid = false;
        for (int i = 0; i < dim; ++i)
        {
            char ch = line[i];
            if (ch == '0')
            {
                if (size)
                {
                    if (valid)
                        spaces.emplace_back(pair<int, int>(pointer, size));
                    size = 0;
                    pointer = i + 1;
                    valid = false;
                }
                else
                    pointer++;
            }
            else if (ch == '-')
            {
                valid = true;
                size++;
            }
            else
                size++;
        }

        if (size)
            spaces.emplace_back(pair<int, int>(pointer, size));

        return spaces;
    }

    // ===== rules enforcers =====
    void maxConstraintRule(int line, int start = 0, int length = -1, bool willComplete = false)
    {
        if (completion[line])
            return;

        if (length == -1)
            length = dim;

        vector<int> *constraint = &constraintsVec[line];
        int conSize = constraint->size();

        if (sumConstraintMinSpace(constraint) == length)
        {
            for (int i = 0; i < conSize; ++i)
            {
                int size = constraint->at(i);
                for (int j = 0; j < size; ++j)
                    if (board.setCellByLine(line, j + start, true))
                        mutated.enqueue(line);

                start += size + 1;
            }

            isComplete(line, willComplete);
        }
    }

    void constraintPaddingRule(int line, int start = 0, int length = -1)
    {
        if (completion[line])
            return;

        if (length == -1)
            length = dim;

        vector<int> *constraint = &constraintsVec[line];
        int conSize = constraint->size();

        for (int i = 0; i < conSize; ++i)
        {
            int pre = start + sumConstraintMinSpace(constraint, 0, i) + 1;
            int remaining = length - pre - sumConstraintMinSpace(constraint, i + 1) - 1;
            int gap = remaining - constraint->at(i);
            int pointer = pre + gap;
            for (int j = 0; j < remaining - gap * 2; ++j)
                if (board.setCellByLine(line, j + pointer, true))
                    mutated.enqueue(line);
        }

        isComplete(line);
    }

    void edgeExtensionRule(int line, string target)
    {
        if (completion[line])
            return;

        vector<int> *constraint = &constraintsVec[line];
        int conSize = constraint->size();

        pair<int, int> positionPair = getFirstMarkedEdgePosition(target);

        if (positionPair.first != -1)
            for (int i = 1; i < constraint->at(0); ++i)
                if (board.setCellByLine(line, positionPair.first + i, true))
                    mutated.enqueue(line);

        if (positionPair.second != -1)
            for (int i = 1; i < constraint->at(conSize - 1); ++i)
                if (board.setCellByLine(line, positionPair.second - i, true))
                    mutated.enqueue(line);

        isComplete(line);
    }

    void NegativePruningRule(int line, string target)
    {
        if (completion[line])
            return;

        for (int i = 0; i < dim; ++i)
            if (target[i] == '-')
            {
                string temp = target;
                temp[i] = '1';
                if (!regex_match(temp, constraintsRegexLoose[line]))
                    if (board.setCellByLine(line, i, false))
                        mutated.enqueue(line);
            }

        isComplete(line);
    }

    void certainRemainingRule(int line, string target)
    {
        if (completion[line])
            return;

        int ones = 0;
        vector<int> remainings;
        for (int i = 0; i < dim; ++i)
        {
            if (target[i] == '-')
                remainings.emplace_back(i);
            else if (target[i] == '1')
                ones++;
        }

        if (ones + remainings.size() == constraintSum[line])
        {
            for (int pos : remainings)
                board.setCellByLine(line, pos, true);

            isComplete(line, false, false);
        }
    }

    // ===== completition checkers =====
    void updateConstraints(int line)
    {
        if (completion[line])
        {
            constraintsVec[line] = vector<int>{};
            return;
        }

        string target = board.read(line);
        vector<int> constraint = cpy(constraintsVecOg[line]);

        int counter = 0;
        for (int i = 0; i < dim; ++i)
        {
            char ch = target[i];
            if (ch == '1')
                counter++;
            else if (ch == '0')
            {
                if (counter)
                {
                    constraint.erase(constraint.begin());
                    counter = 0;
                }
            }
            else
                break;
        }
        if (counter == constraint[0])
            constraint.erase(constraint.begin());

        counter = 0;
        for (int i = dim - 1; i >= 0; --i)
        {
            char ch = target[i];
            if (ch == '1')
                counter++;
            else if (ch == '0')
            {
                if (counter)
                {
                    constraint.pop_back();
                    counter = 0;
                }
            }
            else
                break;
        }
        if (counter == constraint[constraint.size() - 1])
            constraint.pop_back();

        constraintsVec[line] = constraint;
    }

    bool isComplete(int line, bool checkingOverride = false, bool requireFill = true)
    {
        if (completion[line])
            return true;

        string target = board.read(line);
        if (checkingOverride || regex_match(target, constraintsRegexAbs[line]))
        {
            if (requireFill)
                for (int i = 0; i < dim; ++i)
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
        dim = dimIn;
        dim2 = dimIn * 2;
        board = Board(dim);

        // initialise constraints data structures
        constraintSum = vector<int>(dim2, 0);

        constraintsVec = vector<vector<int>>(dim2, vector<int>());
        constraintsVecOg = vector<vector<int>>(dim2, vector<int>());

        regex DEF("x");
        constraintsRegexLoose = vector<regex>(dim2, DEF);
        constraintsRegexAbs = vector<regex>(dim2, DEF);

        // initialise mutated pruning queue
        mutated = DIMQ(dim2);

        // initialise completion maintainers
        completion = vector<bool>(dim2, false);
    }

    // set constraints
    void setRowConstraints(int index, vector<int> constraints)
    {
        constraintsVec[index] = constraints;
        constraintsVecOg[index] = cpy(constraints);

        string abs = "^[0|-]*";
        string loose = "^[0|-]*";

        int size = constraints.size();
        for (int i = 0; i < size; ++i)
        {
            constraintSum[index] += constraints[i];
            abs += "1{" + to_string(constraints[i]) + (i + 1 < size ? "}[0|-]+" : "}");
            loose += "[1|";
            loose += (size > 1 ? "0|" : "");
            loose += "-]{" + to_string(constraints[i]) + (i + 1 < size ? "}[0|-]+" : "}");
        }

        constraintsRegexAbs[index] = regex(abs + "[0|-]*$");
        constraintsRegexLoose[index] = regex(loose + "[0|-]*$");

        if (size > maxConstraintNum)
            maxConstraintNum = size;
    }

    void setColConstraints(int col, vector<int> constraints)
    {
        setRowConstraints(col + dim, constraints);
    }

    // solve board
    void solve()
    {
        bool complete = false;
        int iteration = 0;

        while (!complete && iteration < 1)
        {
            for (int line = 0; line < dim2; ++line)
            {
                vector<pair<int, int>> spaces = getAllContinuousSpace(board.read(line));
                if (spaces.size() == 1)
                {
                    maxConstraintRule(line, spaces[0].first, spaces[0].second);

                    if (iteration == 0 && spaces[0].second > 2)
                        constraintPaddingRule(line, spaces[0].first, spaces[0].second);
                }
            }

            for (int line = 0; line < dim2; ++line)
            {
                string target = board.read(line);

                edgeExtensionRule(line, target);
                NegativePruningRule(line, target);
                certainRemainingRule(line, target);

                updateConstraints(line);
            }

            complete = true;
            for (bool lineComplete : completion)
                complete *= lineComplete;
                
            iteration++;
        }
    }

    // ===== getters for auxiliary helper functions =====
    string getBoard()
    {
        return board.getBoardAsString();
    }

    int getDim()
    {
        return dim;
    }

    int getMaxConstraintNum()
    {
        return maxConstraintNum;
    }

    vector<vector<int>> getConstraints()
    {
        return constraintsVecOg;
    }
};

#endif
