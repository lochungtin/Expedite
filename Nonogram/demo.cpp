#include <iostream>
#include <chrono>

#include "game.h"

using std::cout;
using std::endl;

int main()
{
    Game game = Game(15);

    { // set line constraints
        game.setRowConstraints(0, vector<int>{5});
        game.setRowConstraints(1, vector<int>{2, 2});
        game.setRowConstraints(2, vector<int>{2, 2});
        game.setRowConstraints(3, vector<int>{3, 1, 5});
        game.setRowConstraints(4, vector<int>{3, 3});
        game.setRowConstraints(5, vector<int>{3, 4, 1});
        game.setRowConstraints(6, vector<int>{4, 2, 1});
        game.setRowConstraints(7, vector<int>{3, 3, 2});
        game.setRowConstraints(8, vector<int>{2, 5});
        game.setRowConstraints(9, vector<int>{1, 3});
        game.setRowConstraints(10, vector<int>{2, 1, 1});
        game.setRowConstraints(11, vector<int>{11});
        game.setRowConstraints(12, vector<int>{2, 1, 2});
        game.setRowConstraints(13, vector<int>{15});
        game.setRowConstraints(14, vector<int>{1, 2, 1});

        game.setColConstraints(0, vector<int>{2});
        game.setColConstraints(1, vector<int>{3, 2});
        game.setColConstraints(2, vector<int>{5, 3});
        game.setColConstraints(3, vector<int>{7, 2, 1});
        game.setColConstraints(4, vector<int>{3, 6, 1});
        game.setColConstraints(5, vector<int>{2, 1, 1});
        game.setColConstraints(6, vector<int>{1, 1, 1, 1});
        game.setColConstraints(7, vector<int>{1, 1, 2, 1, 1});
        game.setColConstraints(8, vector<int>{1, 1, 9});
        game.setColConstraints(9, vector<int>{2, 1, 5, 1, 2});
        game.setColConstraints(10, vector<int>{5, 4, 2});
        game.setColConstraints(11, vector<int>{3, 2, 1, 1});
        game.setColConstraints(12, vector<int>{5, 3});
        game.setColConstraints(13, vector<int>{2});
        game.setColConstraints(14, vector<int>{2});
    }
    game.print();

    // // solve game
    // Solver solver = Solver(&game);

    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // int iterations = solver.run();
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // game.print();

    // print running time
    // cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] | Iterations: " << iterations << endl;
}
