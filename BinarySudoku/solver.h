#include "game.h"
#include "../headers/binaryboard.h"

class Solver
{
private:
    Game *game;
    BinaryBoard *board;

public:
    Solver(Game *game) : game(game)
    {
        board = game->getBoard();
    }

    void run()
    {
    }
};
