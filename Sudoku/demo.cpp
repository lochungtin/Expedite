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
    if (0)
    { // mid
        game.setRowWithString(0, "6--4-----");
        game.setRowWithString(1, "-283-96-4");
        game.setRowWithString(2, "19-26-3-5");
        game.setRowWithString(3, "2----48--");
        game.setRowWithString(4, "4---92--3");
        game.setRowWithString(5, "-------1-");
        game.setRowWithString(6, "-52--17--");
        game.setRowWithString(7, "-----3---");
        game.setRowWithString(8, "-61-452-9");
    }
    else
    { // hard
        game.setRowWithString(0, "59-18-7-6");
        game.setRowWithString(1, "6----21-8");
        game.setRowWithString(2, "2--------");
        game.setRowWithString(3, "--4--9-6-");
        game.setRowWithString(4, "----1-2--");
        game.setRowWithString(5, "-1---6--5");
        game.setRowWithString(6, "--9----8-");
        game.setRowWithString(7, "--8965--7");
        game.setRowWithString(8, "4-------2");
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
