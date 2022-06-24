#ifndef PRINTER_H
#define PRINTER_H

#include <iomanip>

#include "game.h"

using std::dec;
using std::hex;

// print board
void printBoard(Game game)
{
    string board = game.getBoard();
    int dim = game.getDim();
    int cellCount = dim * dim;

    string row = "+";
    for (int i = 0; i < dim; ++i)
        row += "---+";
    row += "\n";

    for (int i = 0; i < cellCount; ++i)
    {
        if (i % dim == 0)
        {
            if (i > 0)
                cout << "\n";
            cout << row << "|";
        }
        cout << " " << (game.getBoard()[i] == '-' ? " " : (game.getBoard()[i] == '0' ? "x" : "█")) << " |";
    }
    cout << "\n" + row << endl;
}

// print board along with constraints on the side
void printBoardWithConstraints(Game game)
{
    int dim = game.getDim();
    int maxConstraintNum = game.getMaxConstraintNum();
    int maxDim = maxConstraintNum + dim;

    string rRow = "+";
    string sRow = "+";
    for (int i = 0; i < maxDim; ++i)
    {
        rRow += "---+";
        if (i < maxConstraintNum)
            sRow += "---+";
        else
            sRow += "===+";
    }
    rRow += "\n";
    sRow += "\n";

    vector<vector<int>> constraints = game.getConstraints();

    // print column constraints
    cout << rRow << hex;
    for (int i = 0; i < maxConstraintNum; ++i)
    {
        for (int j = 0; j < maxDim; ++j)
        {
            if (j < maxConstraintNum)
                cout << "|░░░";
            else
            {
                int k = j - maxConstraintNum;
                int l = i - maxConstraintNum + constraints[k + dim].size();
                if (l >= 0)
                    cout << "| " << constraints[k + dim][l] << " ";
                else
                    cout << "|   ";
            }
        }
        if (i + 1 < maxConstraintNum)
            cout << "|\n" + rRow;
    }
    cout << "|\n" + sRow;

    for (int i = 0; i < dim; ++i)
    {
        // print row constraints
        for (int j = 0; j < maxConstraintNum; ++j)
        {
            int l = j - maxConstraintNum + constraints[i].size();
            if (l >= 0)
                cout << "| " << constraints[i][l] << " ";
            else
                cout << "|   ";
        }
        cout << "║";

        // print data
        for (int j = 0; j < dim; ++j)
        {
            int k = i * dim + j;
            cout << " " << (game.getBoard()[k] == '-' ? " " : (game.getBoard()[k] == '0' ? "x" : "█")) << " ";
            if (j + 1 < dim)
                cout << "|";
        }

        if (i + 1 < dim)
            cout << "║\n" + rRow;
    }
    cout << "║\n" + sRow << dec << endl;
}

#endif