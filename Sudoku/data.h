#ifndef DATA_H
#define DATA_H

int subgrid2index(int subGridNum, int relIndex)
{
    return subGridNum * 3 + (subGridNum / 3) * 18 + (relIndex / 3) * 9 + relIndex % 3;
}

int index2subgrid(int index)
{
    return (index / 27) * 3 + (index / 3) % 3;
}

#endif
