#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>

#include "../headers/binaryboard.h"
#include "../headers/printer.h"

using std::cout;
using std::endl;
using std::map;
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
        printBoard(board, map<char, string>{{'-', " "}, {'0', "░"}, {'1', "▓"}});
    }
};

#endif
