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
        game.setRowWithString(0, "185------");
        game.setRowWithString(1, "7----93-4");
        game.setRowWithString(2, "-9-12--8-");
        game.setRowWithString(3, "26---31--");
        game.setRowWithString(4, "-5--1--23");
        game.setRowWithString(5, "-1-5--4-6");
        game.setRowWithString(6, "93---5841");
        game.setRowWithString(7, "--------9");
        game.setRowWithString(8, "--8-412--");
    }
    else
    { // hard
        game.setRowWithString(0, "-47------");
        game.setRowWithString(1, "15---6---");
        game.setRowWithString(2, "-6-78-5--");
        game.setRowWithString(3, "-8--3---9");
        game.setRowWithString(4, "-2-81-4-3");
        game.setRowWithString(5, "------8-7");
        game.setRowWithString(6, "-----2---");
        game.setRowWithString(7, "--5-----1");
        game.setRowWithString(8, "-92-45-68");
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
