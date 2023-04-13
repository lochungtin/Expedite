from pathlib import Path
from random import choices


class Game:
    def __init__(self, file_index):
        path = Path("./MineSweeper/configs/config_{}.txt".format(file_index))
        with open(path, "r") as conf:
            row, col, num = conf.readlines()[0].split(" ")
        
        self.row, self.col, self.num = int(row), int(col), int(num)
        self.board = [[" " for _ in range(self.col)] for _ in range(self.row)]
        self.data = [[0 for _ in range(self.col)] for _ in range(self.row)]
        self.seeded = False

    def expand(self, row, col):
        def neighbours(r, c):
            shift = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
            return list(filter(
                lambda y: self.row > y[0] >= 0 and self.col > y[1] >= 0, 
                map(lambda x: (x[0] + r, x[1] + c), shift)))
        
        if not self.seeded:
            self.seeded = True
            n = neighbours(row, col)
            n.append((row, col))
            cells = [(r, c) for r in range(self.row) for c in range(self.col) if (r, c) not in n]
            self.mines = choices(cells, k=self.num)
            for (mr, mc) in self.mines:
                self.data[mr][mc] = 9

            for r in range(self.row):
                for c in range(self.col):
                    count = sum(map(lambda x: self.data[x[0]][x[1]] == 9, neighbours(r, c)))
                    if self.data[r][c] != 9:
                        self.data[r][c] = count
    
        def reveal(r, c):
            self.board[r][c] = self.data[r][c]
            if self.data[r][c] == 0:
                for (nr, nc) in neighbours(r, c):
                    if self.board[nr][nc] == " ":
                        reveal(nr, nc)
        
        reveal(row, col)


    def flag(self, row, col):
        pass

    def setCell(self, row, col, value):
        if self.board[row][col] == " ":
            self.board[row][col] = str(value * 1)
            return True
        return False

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        row = "+" + "".join("---+" for _ in range(self.col)) + "\n"
        rt = [row + "|" + "".join(f" {c} |" for c in l) for l in self.board]
        return "\n".join(rt + [row[0 : len(row) - 1]])
