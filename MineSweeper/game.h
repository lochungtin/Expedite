#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "../printer.h"
#include "../board.h"

using std::cout;
using std::endl;
using std::map;
using std::random_shuffle;
using std::vector;

class Game
{
private:
    int width;
    int height;
    int size;

    bool unseeded = true;
    int mineCount;
    map<int, bool> mines;

    Board board;
    Board databoard;

    vector<int> shiftMatrix = vector<int>(8);
    vector<int> ptlocs;

public:
    Game(int width, int height, int mineCount) : width(width), height(height), mineCount(mineCount)
    {
        size = width * height;
        board = Board(width, height);
        databoard = Board(width, height);

        // create neighbour shift matrix
        shiftMatrix[0] = -width - 1;
        shiftMatrix[1] = -width;
        shiftMatrix[2] = -width + 1;
        shiftMatrix[3] = -1;
        shiftMatrix[5] = 1;
        shiftMatrix[6] = width - 1;
        shiftMatrix[7] = width;
        shiftMatrix[8] = width + 1;

        // create potential mine locations
        ptlocs = vector<int>(size);
        for (int i = 0; i < size; ++i)
            ptlocs[i] = i;
        random_shuffle(ptlocs.begin(), ptlocs.end());
    }

    bool select(int row, int col)
    {
        int clickedIndex = row * width + col;

        // initial seeding procedure based on first click
        if (unseeded)
        {
            unseeded = false;
            int index = 0;
            for (int i = 0; i < mineCount; ++i)
            {
                // shift to next cell if location is current click
                if (clickedIndex == ptlocs[index++])
                    index++;
                mines.emplace(ptlocs[index], false);
            }
        }

        // check for mine on selected cell
        return false;
    }

    bool flag(int row, int col)
    {
        return false;
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
