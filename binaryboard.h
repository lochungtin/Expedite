#ifndef BINARYBOARD_H
#define BINARYBOARD_H

#include <string>
#include "board.h"

using std::string;

class BinaryBoard : public Board
{
protected:
    int dim;

public:
    BinaryBoard() {}
    BinaryBoard(int dim) : Board(dim, dim), dim(dim) {}

    /**
     * @brief Get the dimensions of the board
     *
     * @return board dimension
     */
    int getDim()
    {
        return dim;
    }

    /**
     * @brief Read lines from board string separated by width
     *
     * @param line line index [0 - width * 2)
     * @return line string
     */
    string readLine(int line)
    {
        if (line / dim)
        {
            int col = line % dim;

            string rt = string(dim, ' ');
            for (int i = 0; i < dim; ++i)
                rt[i] = board[col + dim * i];

            return rt;
        }
        else
            return board.substr(line * dim, dim);
    }

    /**
     * @brief Set cell value indexed by line and relative index to line
     *
     * @param line  line index
     * @param index position index relative to line
     * @param mark  symbol (0 or 1)
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCellByLine(int line, int index, bool mark)
    {
        if (line / dim)
            return setCellByIndex(index * dim + (line % dim), mark);
        else
            return setCellByIndex(line * dim + index, mark);
    }
};

#endif
