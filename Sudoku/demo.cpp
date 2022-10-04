#include <iostream>
#include <chrono>

#include "game.h"
#include "solver.h"

using std::cout;
using std::endl;

int main()
{
    // create game
    Game game = Game();

    { // load game board with preset cells
        game.setRowWithString(0, "9-62---5-");
        game.setRowWithString(1, "--1-----9");
        game.setRowWithString(2, "--------6");
        game.setRowWithString(3, "1-94-68--");
        game.setRowWithString(4, "-32--5-6-");
        game.setRowWithString(5, "76413-59-");
        game.setRowWithString(6, "6---73-4-");
        game.setRowWithString(7, "-9-----7-");
        game.setRowWithString(8, "4--6-2915");
    }
    game.print();

    // solve game;
    Solver solver = Solver(&game);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int iterations = solver.run();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    game.print();

    cout << game.getBoard()->read() << endl;

    // print running time
    cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] | Iterations: " << iterations << endl;
}
