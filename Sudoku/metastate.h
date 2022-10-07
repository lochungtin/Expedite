#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class MetaState
{
public:
    bool possibles[9][9][9];
    bool set[9][9];

    MetaState()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                set[i][j] = false;
                for (int k = 0; k < 9; ++k)
                    possibles[i][j][k] = true;
            }
    }

    void boardSet(int row, int col, int index)
    {
        set[row][col] = true;

        int subRowOffset = (row / 3) * 3;
        int subColOffset = (col / 3) * 3;

        for (int i = 0; i < 9; ++i)
        {
            if (col != i && !set[row][i])
                possibles[row][i][index] = false;
            if (row != i && !set[i][col])
                possibles[i][col][index] = false;

            int subRow = i / 3 + subRowOffset;
            int subCol = i % 3 + subColOffset;

            if (subRow != row && subCol != col && !set[subRow][subCol])
                possibles[subRow][subCol][index] = false;
        }

        for (int i = 0; i < 9; ++i)
            possibles[row][col][i] = (i == index);
    }

    vector<int> getPossibles(int row, int col)
    {
        vector<int> rt;
        for (int i = 0; i < 9; ++i)
            if (possibles[row][col][i])
                rt.emplace_back(i + 1);

        return rt;
    }

    bool incomplete()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (!set[i][j])
                    return true;

        return false;
    }

    void listAll()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                if (set[i][j])
                    cout << "[x] - ";
                else
                    cout << "[ ] - ";

                cout << "(" << i << ", " << j << ") - [ ";
                for (int k = 0; k < 9; ++k)
                    cout << possibles[i][j][k] << " ";

                cout << "]\n";
            }
    }

    void listIncomplete()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (!set[i][j])
                {
                    cout << "(" << i << ", " << j << ") - [ ";
                    for (int k = 0; k < 9; ++k)
                        cout << possibles[i][j][k] << " ";

                    cout << "]\n";
                }
    }
};