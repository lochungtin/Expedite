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
     * @brief Set cell value indexed by row and column
     *
     * @param row   row index
     * @param col   column index
     * @param mark  symbol (0 or 1)
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCellByCoordinate(int row, int col, bool mark)
    {
        return Board::setCellByCoordinate(row, col, (mark ? '1' : '0'));
    }

    /**
     * @brief Set cell value indexed by absolute index
     *
     * @param index board string index
     * @param mark  symbol (0 or 1)
     * @return true     - cell set complete
     * @return false    - cell already occupied
     */
    bool setCellByIndex(int index, bool mark)
    {
        return Board::setCellByIndex(index, (mark ? '1' : '0'));
    }
};

#endif
