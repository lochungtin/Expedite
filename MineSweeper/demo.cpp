#include <iostream>
#include <chrono>
#include <time.h>

#include "game.h"
#include "solver.h"

using std::cout;
using std::endl;

int main()
{
    // srand(time(NULL));

    // create game
    Game game = Game(9, 9, 9);
    game.select(5, 5);

    cout << game.flag(51) << endl;
    cout << game.flag(11) << endl;
    cout << game.flag(69) << endl;
    cout << game.flag(24) << endl;
    cout << game.flag(35) << endl;
    cout << game.flag(17) << endl;
    cout << game.flag(36) << endl;
    cout << game.flag(26) << endl;
    cout << game.flag(50) << endl;

    // solve game
    // Solver solver = Solver(&game);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int iterations = 0; // solver.run();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    game.print();

    // print running time
    cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] | Iterations: " << iterations << endl;
}
