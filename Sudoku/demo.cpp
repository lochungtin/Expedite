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

    // load game board with preset cells
    { // hard
        game.setRowWithString(0, "1-9------");
        game.setRowWithString(1, "7----6--5");
        game.setRowWithString(2, "86----4--");
        game.setRowWithString(3, "---1--26-");
        game.setRowWithString(4, "-----2---");
        game.setRowWithString(5, "--1795---");
        game.setRowWithString(6, "9---2--4-");
        game.setRowWithString(7, "-1---7-3-");
        game.setRowWithString(8, "--53----9");
    }

    game.print();

    // solve game;
    Solver solver = Solver(&game);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int iterations = solver.run();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    game.print();

    // print running time
    cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] | Iterations: " << iterations << endl;
}
