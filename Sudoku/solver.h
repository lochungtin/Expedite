#include <stack>
#include <utility>

#include "game.h"
#include "metastate.h"
#include "../board.h"

using std::pair;
using std::stack;

class Solver
{
private:
    // ===== variables =====
    Game *game;
    stack<pair<Board, MetaState>> states;

    void analyseMetaState()
    {
        Board *topBoard = &states.top().first;
        MetaState *topMetaState = &states.top().second;

        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                char ch = topBoard->readCell(i * 9 + j);
                if (ch != '-')
                {
                    topMetaState->set[i][j] = true;

                    int val = ch - '1';

                    for (int h = 0; h < 9; ++h)
                        if (h != i)
                            topMetaState->possibles[i][h][val] = false;
                }
            }

        for (int j = 0; j < 9; ++j)
        {
            for (int k = 0; k < 9; ++k)
                cout << topMetaState->possibles[5][j][k] << " ";
            cout << "\n";
        }
        cout << endl;
    }

public:
    Solver(Game *game) : game(game)
    {
        states.push(pair<Board, MetaState>{*game->getBoard(), MetaState()});
    }

    int run()
    {
        analyseMetaState();
        return 0;
    }
};
