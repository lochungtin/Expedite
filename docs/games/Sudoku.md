# Sudoku Solver

## Game

The aim of a game of sudoku is to fill the 9x9 grid will numbers from 1 to 9 in a fashion where no row, column, or sub-grid (3x3 squares) have repeating digits.

The game starts with a few seeded numbers and the player will used the seeded numbers to deduce the remaining digits on the grid.

## Rules

1. No row, column, or subgrid can have repeating digits
2. A subgrid is a 3x3 grid, the 9x9 grid is composed of 9 3x3 subgrids.

## Algorithm

The intuition of the algorithm is to iteratively loop over each _non-filled_ cell and reduced the number of possible digits using a few rules and patterns. Once a cell has only one possible digit, it is filled and set, and the process repeats for all remaining cells.

The set of rules here is not enough to solve all puzzles, so guessing and save states have to be used when random guesses must be made. If the guess is incorrect, the solver will revert back to the previous save state and try another value of the cell.

### Patterns

**Pattern 1**

-   Requirement
    -   A possible value in a subgrid only spans one row or column
-   Action
    -   Remove the value from other rows or columns that intersect the target subgrid

**Pattern 2**

-   Requirement
    -   In any pair of 2 cells (_A_, _B_) from the same row, column, or subgrid, the 2 cells have 2 possible values and these values are identical for the 2 selected cells
        -   _P_a1 == P_b1 && P_a2 == P_b2_
-   Action
    -   Remove _P_a1_ and _P_a2_ from all the other cells (_Cell != A || Cell != B_)

**Pattern 3**

-   Requirement
    -   The cell contains a possible value that no other cell in that row, column, or subgrid contains
-   Action
    -   Set the cell with the "unique" possible value

### Algorithm Structure

Highly abstracted algorithm pseudo-code

```clike
bool possibles[9][9][9] = { true };
bool gameComplete = false;

for (auto cell : grid)
    if (filled(cell)) {
        removeValueFromRow(cell.value, cell.row);
        removeValueFromRow(cell.value, cell.col);
    }

while (gameComplete) {
    enforcePattern1();
    enforcePattern2();
    enforcePattern3();

    for (auto cell : grid)
        if (onlyHaveOnePossibleValue(cell))
            fillCellWithRemaining(cell);

    gameComplete = validateGame();
}
```

## Config Setup

The following is an example config file.

```
1-----2--
7-4-6----
--9-8--3-
---1-6-8-
-12---4--
---7---5-
-4------3
5---729--
----98---
```

-   `-` represent blank tiles
-   Numbers correspond to originally numbered tiles
