#include "game.h"

int main()
{
    Game g = Game(1, 2, 3, 4);
    cout << g.guess(5, 1, 3, 4) << endl;
}
