#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

#include "../headers/binaryboard.h"

using std::cout;
using std::endl;
using std::string;

class Game
{
private:
    int dim;
    BinaryBoard board;

public:
    Game(int dim) : dim(dim), board(dim) {}

    bool setCell(int row, int col, bool value)
    {
        return board.setCellByCoordinate(row, col, value);
    }

    void print()
    {
        string row = "+";
        for (int i = 0; i < dim; ++i)
            row += "---+";

        cout << row << "\n|";
        for (int i = 0; i < dim; ++i)
        {
            for (int j = 0; j < dim; ++j)
            {
                char cell = board.read(i, j);
                if (cell == '-')
                    cout << "   |";
                else
                    cout << (cell == '0' ? " ░ |" : " ▓ |");
            }

            cout << "\n" + row + (i == dim - 1 ? "" : "\n|");
        }
        cout << endl;
    }
};

#endif
