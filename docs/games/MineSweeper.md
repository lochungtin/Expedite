# MineSweeper Player

## Game

The aim of a game of minesweeper is to find all the mines hidden in the board. To identify a mine, cells that don't have mines will display the number of mines it has neighbouring itself. The game is finished when all the mines are correctly flagged.

## Rules

There are 2 actions available to the player, **expand** and **flag**.

### Expanding

Expanding is the action to expand unexplored cells to reveal the numbers hidden in the cell. A depth first search algorithm is used to expand the cells and the termination criteria is when a cell with a non-zero number is reached.

If the player expands on a mine, the game is considered a fail. To prevent the first expansion to be on a mine, on an empty grid, the first expansion is used to determine the seeding of the mines. In other words, the mines are placed after the first expansion position is known.

### Flag

Flagging is the action to mark a cell as a mine. Flagging will not reveal if the cell is or is not a mine. The objective of the game is to flag all the cells that have mines correctly.

## Algorithm

The intuition of the algorithm is to iteratively loop over each numbered cell and determine whether or not the neighbouring cells must or must not be a mine. By counting the number of expanded cells and mines around numbered cell, it is possible to determine whether or not a neighbouring cell is or is not a mine.

### Logic

Let $v_c$ be the number of mines around the cell $c$

Let $E_c$ be the set of expanded neighbours of cell $c$

Let $M_c$ be the set of neighbouring cells of cell $c$ that are / marked as mines

Let $U_c$ be the set of unexpanded neighbours of cell $c$

-   $|E_c| + |U_c| = 8$

**All $U_c$ of cell $c$ must be a mine**

-   Condition
    -   $|U_c| = v_c - |M_c|$

**All $U_c$ cell $c$ must not be a mine**

-   Condition
    -   $|M_c| = v_c$

### Algorithm Structure

Highly abstracted algorithm pseudo-code

```cpp
bool hasBoardUpdated;
while (game.isFinished()) {
    hasBoardUpdated = false;
    for (auto cell : cells) {
        if (cell.notExpanded()) {
            if (mustBeMine(cell)) {
                game.flag(cell)
                hasBoardUpdated = true;
            }
            if (mustNotBeMine(cell)) {
                game.expand(cell)
                hasBoardUpdated = true;
            }
        }
    }
    if (!hasBoardUpdated) {
        isMine = game.expandRandomCell();
        while (isMine) {
            game.revert();
            isMine = game.expandRandomCell();
        }
    }
}
```

## Config Setup
