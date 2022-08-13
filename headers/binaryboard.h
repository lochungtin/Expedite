#ifndef BINARYBOARD_H
#define BINARYBOARD_H

#include <string>

using std::string;

class BinaryBoard
{
private:
    int dim;
    string board = "";

public:
    BinaryBoard() {}
    BinaryBoard(int dim) : dim(dim)
    {
        board = string(dim * dim, '-');
    }

    /**
     * @brief Get the dimensions of the board
     *
     * @return board dimension
     */
    int getDim() const
    {
        return dim;
    }

    /**
     * @brief Read individual characters from board string
     *
     * @param row row index relative to board
     * @param col col index relative to board
     * @return indexed character
     */
    char read(int row, int col) const
    {
        return board[row * dim + col];
    }

    /**
     * @brief Read lines from board string separated by dim length
     *
     * @param line line index [0 - dim * 2)
     * @return line string
     */
    string read(int line) const
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
     * @brief Read the entire board string
     *
     * @return complete board string
     */
    string read() const
    {
        return board;
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
        return setCellByIndex(row * dim + col, mark);
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
        if (board[index] != '-')
            return false;

        board[index] = (mark ? '1' : '0');
        return true;
    }
};

#endif
