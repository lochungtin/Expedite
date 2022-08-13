#include <regex>
#include <utility>
#include <vector>

#include "game.h"
#include "../binaryboard.h"

using std::pair;
using std::regex;
using std::sregex_iterator;
using std::vector;

class Solver
{
private:
    // ===== variables =====
    Game *game;
    BinaryBoard *board;

    int dim;

public:
    Solver(Game *game) : game(game)
    {
        // set board pointer
        board = game->getBoard();

        // store dimension
        dim = board->getDim();
    }

    int run()
    {
        int iterations = 0;
        return iterations;
    }
};
