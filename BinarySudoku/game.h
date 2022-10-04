#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>

#include "../binaryboard.h"
#include "../printer.h"

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

    /**
     * @brief Set cell value indexed by row and column to board object
     *
     * @param row   row index
     * @param col   column index
     * @param value symbol (0 or 1)
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCell(int row, int col, bool value)
    {
        return board.setCellByCoordinate(row, col, value);
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
     * @brief Prints game board
     */
    void print()
    {
        printBoard(board, map<char, string>{{'-', " "}, {'0', "░"}, {'1', "▓"}});
    }
};

#endif
