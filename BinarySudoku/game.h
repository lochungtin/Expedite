#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::pair;
using std::regex;
using std::sregex_iterator;
using std::string;
using std::vector;

class Game
{
private:
    // dimensions
    int dim;
    int dimH;
    int cellCount;
    int colPadding;

    int cellSetCount;

    // data
    string board;
    vector<vector<vector<int>>> counts;

    // pair of <matching pattern, index offset>
    vector<pair<regex, int>> actionLib;

    // completion
    vector<bool> completion;

    // set cell by index
    bool setCell(int index, bool one)
    {
        // set cell data
        char og = board[index];
        board[index] = one ? '1' : '0';

        int row = index / colPadding;
        int col = index % colPadding;

        // maintain counts
        counts[0][row][0] += (og == '0') * -1 + (one)*1;
        counts[0][row][1] += (og == '1') * -1 + (!one) * 1;

        counts[1][col][0] += (og == '0') * -1 + (one)*1;
        counts[1][col][1] += (og == '1') * -1 + (!one) * 1;

        // maintain completion
        if (counts[0][row][0] + counts[0][row][1] == dim)
            completion[row] = true;

        if (counts[1][col][0] + counts[1][col][1] == dim)
            completion[dim + col] = true;

        bool complete = true;
        for (bool lineComplete : completion)
            complete *= lineComplete;

        return complete;
    }

public:
    // constructor
    Game(int dimIn)
    {
        // maintain dimension data
        dim = dimIn;
        dimH = dimIn / 2;
        cellCount = dimIn * dimIn;
        colPadding = dimIn + 1;

        cellSetCount = 0;

        string row = string(dimIn, '-');

        vector<vector<int>> r;
        vector<vector<int>> c;
        for (int i = 0; i < dim; ++i)
        {
            // initialise board
            board += row + "/";

            // maintain counter data
            r.emplace_back(vector<int>(2, 0));
            c.emplace_back(vector<int>(2, 0));
        }
        counts.emplace_back(r);
        counts.emplace_back(c);

        // create regex expressions of column matching
        string rowGap = string(dimIn, '.');
        actionLib = {
            pair<regex, int>(regex("-11"), 0),                                          // row double left 1
            pair<regex, int>(regex("-00"), 0),                                          // row double left 0
            pair<regex, int>(regex("11-"), 2),                                          // row double right 1
            pair<regex, int>(regex("00-"), 2),                                          // row double right 0
            pair<regex, int>(regex("1-1"), 1),                                          // row gap 1
            pair<regex, int>(regex("0-0"), 1),                                          // row gap 0
            pair<regex, int>(regex("-" + rowGap + "1" + rowGap + "1"), 0),              // col double left 1
            pair<regex, int>(regex("-" + rowGap + "0" + rowGap + "0"), 0),              // col double left 0
            pair<regex, int>(regex("1" + rowGap + "1" + rowGap + "-"), colPadding * 2), // col double right 1
            pair<regex, int>(regex("0" + rowGap + "0" + rowGap + "-"), colPadding * 2), // col double right 0
            pair<regex, int>(regex("1" + rowGap + "-" + rowGap + "1"), colPadding),     // col gap 1
            pair<regex, int>(regex("0" + rowGap + "-" + rowGap + "0"), colPadding),     // col gap 0
        };

        completion = vector<bool>(dim * 2, 0);
    }

    // set cell
    bool setCell(int row, int col, bool one)
    {
        return setCell(row * colPadding + col, one);
    }

    // solve board
    void solve()
    {
        while (true)
        {
            for (int orientation = 0; orientation < 2; ++orientation)
            {
                // half filled mutations
                for (int i = 0; i < dim; ++i)
                    if (!completion[i + dim * orientation] && (counts[orientation][i][0] == dimH) != (counts[orientation][i][1] == dimH))
                        for (int j = 0; j < dim; ++j)
                        {
                            int index = j + i * colPadding;
                            if (orientation)
                                index = i + j * colPadding;
                            if (board[index] == '-')
                                if (setCell(index, counts[orientation][i][1] == dimH))
                                    return;
                        }

                // duplication preventating mutations
                for (int i = 0; i < dim; ++i)
                    if (!completion[i + dim * orientation] && counts[orientation][i][0] == dimH - 1 && counts[orientation][i][1] == dimH - 1)
                        for (int target = 0; target < dim; ++target)
                            if (i != target && counts[orientation][target][0] + counts[orientation][target][1] == dim)
                            {
                                int delta = i - target;
                                if (!orientation)
                                    delta *= colPadding;

                                vector<int> refs;
                                for (int position = 0; position < dim; ++position)
                                {
                                    int r = i * colPadding + position;
                                    if (orientation)
                                        r = position * colPadding + i;
                                    if (board[r] != board[r - delta])
                                        refs.emplace_back(r);
                                }

                                if (refs.size() == 2)
                                    for (int r : refs)
                                        if (setCell(r, board[r - delta] != '0'))
                                            return;
                            }
            }

            // action library mutations
            for (int actionIndex = 0; actionIndex < 12; ++actionIndex)
            {
                vector<int> positions;
                sregex_iterator itr = sregex_iterator(board.begin(), board.end(), actionLib[actionIndex].first);
                for (; itr != sregex_iterator(); ++itr)
                    if (setCell((*itr).position(0) + actionLib[actionIndex].second, actionIndex % 2))
                        return;
            }                
        }
    }

    // print board for debug
    void printBoard()
    {
        string row = "+";
        for (int i = 0; i < dim; ++i)
            row += "---+";

        cout << row << "\n|";
        for (int i = 0; i < cellCount + dim; ++i)
        {
            if (board[i] == '/')
                cout << "\n" + row << (i + 1 < cellCount + dim ? "\n|" : "");
            else if (board[i] == '-')
                cout << "   |";
            else
                cout << (board[i] == '0' ? " ░ |" : " ▓ |");
        }
        cout << endl;
    }

    // print counters
    void printCounter()
    {
        cout << "=== ROW ===\n";
        for (int i = 0; i < dim; ++i)
            cout << "Row[" << i << "] r0: " << counts[0][i][0] << " r1: " << counts[0][i][1] << "\n";

        cout << "=== COL ===\n";
        for (int i = 0; i < dim; ++i)
            cout << "Col[" << i << "] c0: " << counts[1][i][0] << " c1: " << counts[1][i][1] << "\n";
    }
};

#endif
