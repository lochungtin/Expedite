class MetaState
{
public:
    bool sqr[9][9];
    bool row[9][9];
    bool col[9][9];
    bool possibles[9][9][9];

    bool set[9][9];

    MetaState()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                sqr[i][j] = false;
                row[i][j] = false;
                col[i][j] = false;
                set[i][i] = false;
                for (int k = 0; k < 9; ++k)
                    possibles[i][j][k] = true;
            }
    }
};