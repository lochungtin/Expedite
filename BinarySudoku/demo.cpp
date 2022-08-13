#include <iostream>
#include <chrono>

#include "game.h"
#include "solver.h"

using std::cout;
using std::endl;

int main()
{
    // create game
    Game game = Game(12);

    // load game board with preset cells
    {
        game.setCell(0, 2, false);
        game.setCell(0, 4, true);
        game.setCell(0, 8, false);
        game.setCell(0, 10, false);
        game.setCell(0, 11, false);

        game.setCell(1, 2, false);
        game.setCell(1, 3, false);
        game.setCell(1, 5, true);
        game.setCell(1, 7, true);

        game.setCell(2, 6, false);
        game.setCell(2, 10, false);

        game.setCell(3, 3, false);
        game.setCell(3, 8, false);

        game.setCell(4, 0, true);
        game.setCell(4, 2, false);
        game.setCell(4, 6, false);
        game.setCell(4, 8, false);
        game.setCell(4, 9, true);

        game.setCell(5, 0, false);
        game.setCell(5, 3, false);
        game.setCell(5, 4, false);

        game.setCell(7, 3, false);
        game.setCell(7, 4, false);
        game.setCell(7, 8, false);
        game.setCell(7, 9, false);

        game.setCell(8, 0, true);
        game.setCell(8, 7, true);
        game.setCell(8, 10, false);

        game.setCell(9, 1, false);
        game.setCell(9, 4, false);
        game.setCell(9, 6, false);

        game.setCell(10, 3, true);
        game.setCell(10, 7, true);
        game.setCell(10, 9, true);

        game.setCell(11, 4, false);
    }
    game.print();

    // solve game
    Solver solver = Solver(&game);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int iterations = solver.run();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    game.print();

    // print running time
    cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] | Iterations: " << iterations << endl;
}
