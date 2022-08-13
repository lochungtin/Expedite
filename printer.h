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

/**
 * @brief Create a row separator string for printing of given length
 *
 * @param length set length of the row separator
 * @return string - row separator
 */
string createRowString(int length, bool thick = false)
{
    string row = "+";
    for (int i = 0; i < length; ++i)
        row.append(thick ? "===" : "---").append("+");

    return row;
}

/**
 * @brief Prints the given board object
 *
 * @param board             board to be printed
 * @param cMap              character map
 * @param willPrintOverride enable print to terminal (default true)
 * @return string - formatted board string
 */
string printBoard(BinaryBoard board, map<char, string> cMap, bool willPrintOverride = true)
{
    string returnString = "";

    int dim = board.getDim();

    string rowString = createRowString(dim);
    returnString.append(rowString).append("\n|");

    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
            returnString.append(" ").append(cMap[board.read(i, j)]).append(" |");

        returnString.append("\n").append(rowString).append(i == dim - 1 ? "" : "\n|");
    }

    if (willPrintOverride)
        cout << returnString << endl;

    return returnString;
}

#endif
