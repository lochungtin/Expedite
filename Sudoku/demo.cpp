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
        game.setRowWithString(0, "9-4-653--");
        game.setRowWithString(1, "---2--5-1");
        game.setRowWithString(2, "1-683---4");
        game.setRowWithString(3, "28----745");
        game.setRowWithString(4, "--7-82-9-");
        game.setRowWithString(5, "56-7--83-");
        game.setRowWithString(6, "795-2-61-");
        game.setRowWithString(7, "--3-7-2--");
        game.setRowWithString(8, "-12---47-");
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
