# Kuromasu Solver

## Game

Kuromasu is a binary-determination logic puzzle played on a square grid. The aim of the game is to fill empty cells with values of either `0` or `1` based on information given from neighbouring cells. A complete game should have all the cells filled in a fashion where all the rules are satisfied.

## Rules

The concept of a **connection** is that if a cell is adjacent another cell that is not a blocking cell. The connected property is associative, so if cell `A` is connected to `B` and `B` to `C`, then `A`, `B`, and `C` are all connected.

Adjacency is defined as up, down, left, and right. The diagonals are not included.

1. All the intially numbered cells must have the same number of cells connected to them.

2. Blocking cells are used to segment connected blocks to fit the desired number of connections.

## Algorithm

### Algorithm Structure

Highly abstracted algorithm pseudo-code

```clike
Cell[] stack = game.getNonZeroCells();
while (stack.length > 0) {
    cell = stack.top;
    for (auto end : cell.ends()) {
        end.setAsConnected();
        if (cell.connected() > cell.value)
            end.setAsBlocker();
        else
            end.revertChnages()
    }
    cell.fillConfirmedSpaces();
    if (cell.availableSpace() == cell.value)
        cell.fillAllAvailableSpace();
    if (cell.hasOneAvailableDirection())
        cell.expandRemaining();
    if (cell.complete())
        stack.pop();
}

game.fillEmptyWithBlockers();
```

## Config Setup

The following is an example config file.

```
--6---
-2-x32
-----2
3-----
-4---2
--2--2
```

-   `-` represent blank tiles
-   `x` represent blocking tiles
-   Numbers correspond to originally numbered tiles
