from random import sample

from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.row, self.col, self.num = list(map(int, lines[0].split(" ")))
        self.board = [[" " for _ in range(self.col)] for _ in range(self.row)]
        self.data = [[0 for _ in range(self.col)] for _ in range(self.row)]
        self.flags = {}
        self.seeded = False

    def expand(self, row, col):
        def neighbours(r, c):
            shift = [
                (-1, -1),
                (-1, 0),
                (-1, 1),
                (0, -1),
                (0, 1),
                (1, -1),
                (1, 0),
                (1, 1),
            ]
            return list(
                filter(
                    lambda y: self.row > y[0] >= 0 and self.col > y[1] >= 0,
                    map(lambda x: (x[0] + r, x[1] + c), shift),
                )
            )

        if not self.seeded:
            self.seeded = True
            n = neighbours(row, col)
            n.append((row, col))
            cells = [
                (r, c)
                for r in range(self.row)
                for c in range(self.col)
                if (r, c) not in n
            ]
            self.mines = sample(cells, self.num)
            for (mr, mc) in self.mines:
                self.data[mr][mc] = 9
                self.flags[(mr, mc)] = False

            for r in range(self.row):
                for c in range(self.col):
                    count = sum(
                        map(lambda x: self.data[x[0]][x[1]] == 9, neighbours(r, c))
                    )
                    if self.data[r][c] != 9:
                        self.data[r][c] = count

        if (row, col) in self.mines:
            return False

        def reveal(r, c):
            self.board[r][c] = self.data[r][c]
            if self.data[r][c] == 0:
                for (nr, nc) in neighbours(r, c):
                    if self.board[nr][nc] == " ":
                        reveal(nr, nc)

        reveal(row, col)
        return True

    def flag(self, row, col):
        if self.board[row][col] == " ":
            self.board[row][col] = "x"

        pos = (row, col)
        if pos in self.flags:
            self.flags[pos] = not self.flags[pos]

    def validation(self):
        return all(map(lambda x: x[1], self.flags.items()))
