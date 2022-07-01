#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::pair;
using std::vector;

class Board
{
private:
    int width;
    int height;
    int size;

    vector<int> data;
    vector<vector<int>> neighbours;

public:
    Board(){}
    Board(int widthIn, int heightIn)
    {
        width = widthIn;
        height = heightIn;
        size = widthIn * heightIn;

        data = vector<int>(size, 0);
        neighbours = vector<vector<int>>(size, vector<int>());

        for (int y = 0; y < heightIn; ++y)
            for (int x = 0; x < widthIn; ++x)
                for (int dy = -1; dy < 2; ++dy)
                    for (int dx = -1; dx < 2; ++dx)
                        if (dy != 0 && dx != 0)
                        {
                            int nx = x + dx;
                            int ny = y + dy;

                            if (nx > -1 && nx < widthIn && ny > -1 && ny < heightIn)
                                neighbours[y * width + x].emplace_back(ny * width + nx);
                        }
    }

    int get(int x, int y)
    {
        return data[y * width + x];
    }

    vector<int> neighbours(int x, int y)
    {
        return neighbours[y * width + x];
    }
};

#endif
