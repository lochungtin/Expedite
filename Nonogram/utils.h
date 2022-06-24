#ifndef UTILS_H
#define UTILS_H

#include <vector>

using std::vector;

vector<int> cpy(vector<int> in)
{
    vector<int> out;
    for (int i : in)
        out.emplace_back(i);

    return out;
}

#endif
