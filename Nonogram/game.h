#ifndef GAME_H
#define GAME_H

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../binaryboard.h"
#include "../printer.h"

using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::map;
using std::string;
using std::vector;

class Game
{
private:
    int dim;
    BinaryBoard board;

    // constraints
    int maxConstraintNum = 0;
    vector<vector<int>> rowCon;
    vector<vector<int>> colCon;

public:
    Game(int dim) : dim(dim), board(dim)
    {
        rowCon = vector<vector<int>>(dim);
        colCon = vector<vector<int>>(dim);
    }

    /**
     * @brief Set the row constraints array
     *
     * @param index         index of the row constraint
     * @param constraints   constraint array
     */
    void setRowConstraints(int row, vector<int> constraints)
    {
        rowCon[row] = constraints;

        // update max constraint number
        int size = constraints.size();
        if (size > maxConstraintNum)
            maxConstraintNum = size;
    }

    /**
     * @brief Set the column constraints array
     *
     * @param col           index of the column constraint
     * @param constraints   constraint array
     */
    void setColConstraints(int col, vector<int> constraints)
    {
        colCon[col] = constraints;

        // update max constraint number
        int size = constraints.size();
        if (size > maxConstraintNum)
            maxConstraintNum = size;
    }

    /**
     * @brief Get the pointer to the Board object
     *
     * @return BinaryBoard pointer
     */
    BinaryBoard *getBoard()
    {
        return &board;
    }

    /**
     * @brief Prints game board with constraint information
     */
    void print()
    {
        // generate separator custom rows
        string lRow = createRowString(maxConstraintNum + dim);
        string sRow = createRowString(maxConstraintNum);
        sRow.pop_back();

        // set cout to hex mode
        cout << hex << lRow << "\n";

        // print column constraints
        for (int i = 0; i < maxConstraintNum; ++i)
        {
            // print fill for top left corner
            for (int j = 0; j < maxConstraintNum; ++j)
                cout << "|░░░";
            for (int j = 0; j < dim; ++j)
            {
                // printer padding control
                int k = i - maxConstraintNum + colCon[j].size();
                if (k >= 0)
                    cout << "| " << colCon[j][k] << " ";
                else
                    cout << "|   ";
            }
            // print separator row
            cout << "|\n" + (i + 1 == maxConstraintNum ? (sRow + createRowString(dim, true)) : lRow) + "\n";
        }

        // get grid data
        string printData = printBoard(board, map<char, string>{{'-', " "}, {'0', "░"}, {'1', "▓"}}, false);

        int rowLength = dim * 4 + 2;
        for (int i = 0; i < dim; ++i)
        {
            // print row constraints
            for (int j = 0; j < maxConstraintNum; ++j)
            {
                // printer padding control
                int k = j - maxConstraintNum + rowCon[i].size();
                if (k >= 0)
                    cout << "| " << rowCon[i][k] << " ";
                else
                    cout << "|   ";
            }

            // print grid row data using substrings
            int gridStart = (i * 2 + 1) * rowLength + 1;
            string gridString = printData.substr(gridStart, rowLength - 2);

            cout << "║" + gridString + "\n" + lRow + "\n";
        }

        // change cout back to dec mode and flush
        cout << dec << endl;
    }
};

#endif
