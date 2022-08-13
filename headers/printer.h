#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <map>
#include <string>

#include "binaryboard.h"

using std::cout;
using std::endl;
using std::map;
using std::string;

string createRowString(int length)
{
    string row = "+";
    for (int i = 0; i < length; ++i)
        row += "---+";

    return row;
}

string print(BinaryBoard board, map<char, char> cMap)
{
    string rowString = createRowString(board.)
}

#endif
