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
        game.setRowWithString(0, "--24-----");
        game.setRowWithString(1, "--45-6-2-");
        game.setRowWithString(2, "5167-8---");
        game.setRowWithString(3, "---83--12");
        game.setRowWithString(4, "-28--7-5-");
        game.setRowWithString(5, "--5--9--8");
        game.setRowWithString(6, "---9841-6");
        game.setRowWithString(7, "-43-5----");
        game.setRowWithString(8, "----7---5");
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
