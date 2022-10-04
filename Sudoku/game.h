#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>

#include "../board.h"
#include "../printer.h"

using std::to_string;

class Game
{
private:
    Board board;

public:
    /**
     * @brief Set cell value indexed by row and column to board object
     *
     * @param row   row index
     * @param col   column index
     * @param value value to set
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCell(int row, int col, int value)
    {
        return board.setCellByCoordinate(row, col, value);
    }

    /**
     * @brief Get the pointer to the Board object
     *
     * @return Board pointer
     */
    Board *getBoard()
    {
        return &board;
    }

    /**
     * @brief Prints game board
     */
    void print()
    {
        map<char, string> cmap = {{'-', " "}};
        for (int i = 0; i < 10; ++i)
        {
            string s = to_string(i);
            cmap.insert({s[0], s});
        }
        printBoard(board, cmap);
    }
};

#endif