#ifndef BOARD_H
#define BOARD_H

#include <string>

using std::string;

class Board
{
protected:
    int width;
    int height;
    int size;

    string board = "";

public:
    Board() {}
    Board(int width, int height, char fill = '-') : width(width), height(height)
    {
        size = width * height;
        board = string(size, fill);
    }

    // ===== GET Functions ======
    /**
     * @brief Get the width of the board
     *
     * @return board width
     */
    int getWidth()
    {
        return width;
    }

    /**
     * @brief Get the height of the board
     *
     * @return board height
     */
    int getHeight()
    {
        return height;
    }

    // ===== READ Functions =====
    /**
     * @brief Read lines from board string
     *
     * @param line line index
     * @return line string
     */
    string readLine(int line)
    {
        if (line / width)
        {
            int col = line % width;

            string rt = string(height, ' ');
            for (int i = 0; i < height; ++i)
                rt[i] = board[col + width * i];

            return rt;
        }
        else
            return board.substr(line * width, width);
    }

    /**
     * @brief Read individual characters from board string
     *
     * @param row row index relative to board
     * @param col col index relative to board
     * @return indexed character
     */
    char readCell(int row, int col)
    {
        return board[row * width + col];
    }

    /**
     * @brief Read individual characters from board string
     *
     * @param index index relative to board
     * @return indexed character
     */
    char readCell(int index)
    {
        return board[index];
    }

    /**
     * @brief Read the entire board string
     *
     * @return complete board string
     */
    string read()
    {
        return board;
    }

    /**
     * @brief Read the entire board string column by row
     *
     * @return complete rotated board string
     */
    string readRotated()
    {
        string rBoard = string(size, '-');
        int shift = width * (height - 1);
        for (int i = 0; i < height; ++i)
        {
            int rShift = shift + i;
            for (int j = 0; j < width; ++j)
                rBoard[i * width + j] = board[rShift - width * j];
        }

        return rBoard;
    }

    // ===== SET Functions =====
    /**
     * @brief Set cell value indexed by row and column
     *
     * @param row                   row index
     * @param col                   column index
     * @param val                   character value
     * @param disableCharOverride   prevent override existing character
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCellByCoordinate(int row, int col, char val, bool disableCharOverride = true)
    {
        return setCellByIndex(row * width + col, val, disableCharOverride);
    }

    /**
     * @brief Set cell value indexed by absolute index
     *
     * @param index                 board string index
     * @param val                   character value
     * @param disableCharOverride   prevent override existing character
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCellByIndex(int index, char val, bool disableCharOverride = true)
    {
        if (board[index] != '-' && disableCharOverride)
            return false;

        board[index] = val;
        return true;
    }
};

#endif
