# Binary Sudoku Solver
## Game
The aim of a game of binary sudoku is to fill a square grid of cells with values of either `0` or `1`. There are a few rule as to how the grids can be filled. The finish the game, all the cells of the square grid has to be filled with either a `0` or `1`, none can be left empty, while all rules has to be satisfied.

## Game Rules
There are 3 simple rules for a game of **Binary Sudoku**:
1.  Each row and column has to be made up of the same amount of `0` and `1` cells
2.  No 2 rows or columns can be the same
3.  No 3 consecutive tiles in a row or column can be of the same colour

## Algorithm
The intuition of the algorithm is the iteratively loop over the each row and column and matching patterns of already set cells. Each of the game rule can be converted into a pattern that could be looked for.

### Patterns
**Pattern 1**
- Requirement
	- Half of the cells of a row or column are of the same type (`0` or `1`)
- Action
	- Set remaining empty cells of the selected row or column to the other type

**Pattern 2**
- Requirements
	- Row or column *A* is completely filled and valid
	- Row or column *B* has 2 remaining empty cells
	- *A* and *B* match perfectly except the remaining 2 cells
- Action
	- Fill the remaining cells of *B* opposite to what is in *A*

**Pattern 3**
- Requirements
	- Presence of these pattern in the row or column
		- Patterns (in regex; empty cells are represented with `-`)
			- *Type 0*
				- `-00`, `00-`, `0-0`
			- *Type 1*
				- `-11`, `11-`, `1-1`
- Action
	- Fill the empty cell from the matched pattern with the opposite cell type
		- Fill cell with `1` for *Type 0* patterns
		- Fill cell with `0` for *Type 1* patterns

(Pattern indices correspond to each game rule)

### Algorithm structure
Highly abstracted algorithm pseudo-code
```cpp
bool hasBoardUpdated = true;
while (hasBoardUpdated) {
	hasBoardUpdated = false;

	if (enforcedRule1())
		hasBoardUpdated = true;
	if (enforcedRule2())
		hasBoardUpdated = true;
	if (enforcedRule3())
		hasBoardUpdated = true;
}
```
