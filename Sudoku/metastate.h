#include <iostream>
#include <vector>

#include "data.h"

using std::cout;
using std::endl;
using std::vector;

class MetaState
{
public:
    bool set[81];
    bool possibles[81][9];

    MetaState()
    {
        for (int i = 0; i < 81; ++i)
        {
            set[i] = false;

            for (int j = 0; j < 9; ++j)
                possibles[i][j] = true;
        }
    }

    /**
     * @brief Set the cell with value and maintain possibles
     *
     * @param index cell index
     * @param value value to set
     */
    void setCell(int index, int value)
    {
        // set cell as true
        set[index] = true;

        // remove possibles for neighbouring cells
        removeRowPossibles(index, value);
        removeColPossibles(index, value);
        removeSubPossibles(index, value);

        // remove possibles for set cell
        for (int i = 0; i < 9; ++i)
            possibles[index][i] = (i == index);
    }

    /**
     * @brief Set all instances of "value" to false for given row
     *
     * @param index cell index to omit
     * @param value value to set as false
     */
    void removeRowPossibles(int index, int value)
    {
        int shift = (index / 9) * 9;
        for (int i = 0; i < 9; ++i)
        {
            int position = shift + i;
            if (!set[position] && position != index)
                possibles[position][value] = false;
        }
    }

    /**
     * @brief Set all instances of "value" to false for given column
     *
     * @param index cell index to omit
     * @param value value to set as false
     */
    void removeColPossibles(int index, int value)
    {
        int shift = index % 9;
        for (int i = 0; i < 81; i += 9)
        {
            int position = shift + i;
            if (!set[position] && position != index)
                possibles[position][value] = false;
        }
    }

    /**
     * @brief Set all instances of "value" to false for given subgrid
     *
     * @param index cell index to omit
     * @param value value to set as false
     */
    void removeSubPossibles(int index, int value)
    {
        int subgrid = index2subgrid(index);
        for (int i = 0; i < 9; ++i)
        {
            int position = subgrid2index(subgrid, i);
            if (!set[position] && position != index)
                possibles[position][value] = false;
        }
    }

    /**
     * @brief Get number of remaining possible values of cell
     *
     * @param index cell index
     * @return int  number of remaining possible values
     */
    int getPossibleSize(int index)
    {
        int sum = 0;
        for (int i = 0; i < 9; ++i)
            sum += possibles[index][i];

        return sum;
    }

    /**
     * @brief Get the remaining single possible value of the cell
     * ONLY USE WHEN CERTAIN CELL HAS ONE REMAINIG VALUE LEFT
     *
     * @param index cell index
     * @return int  value
     */
    int getSingle(int index)
    {
        for (int i = 0; i < 9; ++i)
            if (possibles[index][i])
                return i;

        return 0;
    }

    /**
     * @brief Check if board is incomplete
     *
     * @return true
     * @return false
     */
    bool incomplete()
    {
        for (int i = 0; i < 81; ++i)
            if (!set[i])
                return true;

        return false;
    }

    /**
     * @brief List all possible values for incomplete cells
     *
     */
    void listIncomplete()
    {
        for (int i = 0; i < 81; ++i)
            if (!set[i])
            {
                cout << "(" << i << ") - [ ";
                for (int j = 0; j < 9; ++j)
                    cout << possibles[i][j] << " ";

                cout << "]\n";
            }
    }
};