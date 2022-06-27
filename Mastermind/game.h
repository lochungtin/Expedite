#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include "data.h"

using std::cout;
using std::endl;
using std::vector;

class Game
{
private:
    int t0, t1, t2, t3;
    int colorCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};

public:
    Game(int i0 = rand() % 8, int i1 = rand() % 8, int i2 = rand() % 8, int i3 = rand() % 8)
    {
        t0 = i0;
        t1 = i1;
        t2 = i2;
        t3 = i3;

        colorCount[i0]++;
        colorCount[i1]++;
        colorCount[i2]++;
        colorCount[i3]++;
    }

    int guess(int g0, int g1, int g2, int g3)
    {
        int gColorCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        gColorCount[g0]++;
        gColorCount[g1]++;
        gColorCount[g2]++;
        gColorCount[g3]++;

        int colors = 8;
        for (int i = 0; i < 8; ++i)
            colors -= abs(colorCount[i] - gColorCount[i]);

        colors /= 2;

        return RESPONSE_CODE_OFFSET[colors] + (g0 == t0) + (g1 == t1) + (g2 == t2) + (g3 == t3);
    }
};

#endif
