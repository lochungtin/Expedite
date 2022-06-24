#ifndef DIMQ_H
#define DIMQ_H

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class DIMQ
{
private:
    int size;
    int start = 0;
    int end = 0;

    vector<int> data;
    vector<bool> absence;

public:
    DIMQ() {}
    DIMQ(int sizeIn)
    {
        size = sizeIn;

        data = vector<int>(size, 0);
        absence = vector<bool>(size, true);
    }

    void enqueue(int num)
    {
        if (absence[num])
        {
            absence[num] = false;

            data[end] = num;
            end = (end + 1) % size;
        }
    }

    int dequeue()
    {
        int rt = data[start];
        absence[rt] = true;

        start = (start + 1) % size;

        return rt;
    }

    bool isEmpty()
    {
        return start == end;
    }

    void print()
    {
        cout << "[ ";
        for (auto d : data)
            cout << d << " ";
        cout << "]" << endl;
    }
};

#endif