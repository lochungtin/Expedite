# MineSweeper Player

## Game

The aim of a game of minesweeper is to find all the mines hidden in the board. To identify a mine, cells that don't have mines will display the number of mines it has neighbouring itself. The game is finished when all the mines are correctly flagged.

## Game Mechanics

There are 2 actions available to the player, **expand** and **flag**.

### Expanding

Expanding is the action to expand unexplored cells to reveal the numbers hidden in the cell. A depth first search algorithm is used to expand the cells and the termination criteria is when a cell with a non-zero number is reached.

If the player expands on a mine, the game is considered a fail. To prevent the first expansion to be on a mine, on an empty grid, the first expansion is used to determine the seeding of the mines. In other words, the mines are placed after the first expansion position is known.

### Flag

Flagging is the action to mark a cell as a mine. Flagging will not reveal if the cell is or is not a mine. The objective of the game is to flag all the cells that have mines correctly.

## Algorithm
