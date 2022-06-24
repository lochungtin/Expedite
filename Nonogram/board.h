#ifndef BOARD_H
#define BOARD_H

#include <string>

using std::string;

class Board
{
private:
    int dim;
    string board = "";

public:
    Board() {}
    Board(int dimIn)
    {
        dim = dimIn;
        board = string(dim * dim, '-');
    }

    string read(int line)
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

    string getBoardAsString()
    {
        return board;
    }

    bool setCellByLine(int line, int index, bool mark)
    {
        if (line / dim)
            return setCellByIndex(index * dim + (line % dim), mark);
        else
            return setCellByIndex(line * dim + index, mark);
    }

    bool setCellByCoordinate(int row, int col, bool mark)
    {
        return setCellByIndex(row * dim + col, mark);
    }

    bool setCellByIndex(int index, bool mark)
    {
        if (board[index] != '-')
            return false;

        board[index] = (mark ? '1' : '0');
        return true;
    }
};

#endif
