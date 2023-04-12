#ifndef DATA_H
#define DATA_H

#include <cmath>

/**
 * @brief Convert subgrid and relative index to absolute index
 *
 * @param subGridNum    subgrid index
 * @param relIndex      relative index
 * @return absolute index
 */
int subgrid2index(int subGridNum, int relIndex)
{
    return subGridNum * 3 + (subGridNum / 3) * 18 + (relIndex / 3) * 9 + relIndex % 3;
}

/**
 * @brief Convert absolute index to subgrid index
 *
 * @param index absolute index
 * @return subgrid index
 */
int index2subgrid(int index)
{
    return (index / 27) * 3 + (index / 3) % 3;
}

/**
 * @brief Convert pair value to signature value [0-35]
 *
 * @param pair  pair value
 * @return signature value
 */
int pair2signature(int pair)
{
    int f = pair / 10;
    int s = pair % 10;
    return 35 - (16 - f) * (f + 1) / 2 + s - f;
}

/**
 * @brief Convert signature value to pair value
 *
 * @param signature signature value
 * @return pair value
 */
int signature2pair(int signature)
{
    int u = sqrt(2 * signature + 1) + 0.5;
    int f = 8 - u;
    int s = signature + f + 1 - u * (u - 1) / 2;
    return f * 10 + s;
}

#endif
