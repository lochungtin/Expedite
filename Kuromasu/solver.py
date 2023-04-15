class Cell:
    def __init__(self, row, col, val=0, trans=False, block=False):
        self.row = row
        self.col = col
        self.v = val
        self.t = trans
        self.b = block
        self.u = None
        self.d = None
        self.l = None
        self.r = None

    def __str__(self) -> str:
        return self.__repr__()

    def __repr__(self) -> str:
        return "({}, {}) [v{} t{} b{}]".format(
            self.row, self.col, self.v, self.t * 1, self.b * 1
        )


class Solver:
    def __init__(self, game):
        self.g = game
        self.d = game.size
        self.b = game.board

        self.c = {}
        for r, row in enumerate(self.b):
            for c, cell in enumerate(row):
                val, block = 0, False
                if cell == "x":
                    block = True
                elif cell != " ":
                    val = int(cell)
                self.c[(r, c)] = Cell(r, c, val=val, block=block)

        for i in range(self.d - 1):
            j = self.d - i - 1
            for s in range(self.d):
                self.c[(s, i)].r = self.c[(s, i + 1)]
                self.c[(i, s)].d = self.c[(i + 1, s)]
                self.c[(s, j)].l = self.c[(s, j - 1)]
                self.c[(j, s)].u = self.c[(j - 1, s)]

    def run(self):
        iteration = -1
        self.__setCell(0, 0, 0)
        return iteration

    def __setCell(self, row, col, val):
        self.g.setCell(row, col, val)
        self.c[(row, col)].t = val == True
        self.c[(row, col)].b = val == False