#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>

#include "../board.h"
#include "../printer.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

class Game
{
private:
    Board board = Board(9, 9);

public:
    /**
     * @brief Set the board row with string
     *
     * @param row       row index
     * @param rowString row string
     */
    void setRowWithString(int row, string rowString)
    {
        for (int i = 0; i < 9; ++i)
            board.setCellByCoordinate(row, i, rowString[i]);
    }

    /**
     * @brief Set cell value by index to board object
     *
     * @param index cell index
     * @param value value to set
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCell(int index, int value)
    {
        return board.setCellByIndex(index, value + 49);
    }

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
        return board.setCellByCoordinate(row, col, value + 49);
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
        for (int i = 1; i < 10; ++i)
        {
            string str = to_string(i);
            cmap.insert({str[0], str});
        }
        printBoard(board, cmap);
    }
};

#endif