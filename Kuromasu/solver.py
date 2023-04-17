class Cell:
    def __init__(self, row, col, val=0, trans=False, block=False):
        self.row = row
        self.col = col
        self.v = val
        self.t = trans
        self.b = block
        self.n = [None, None, None, None]

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
        self.i = []
        self.c = {}

    def run(self):
        for r, row in enumerate(self.b):
            for c, cell in enumerate(row):
                if cell == "x":
                    self.c[(r, c)] = Cell(r, c, block=True)
                elif cell != " ":
                    self.c[(r, c)] = Cell(r, c, val=int(cell), trans=True)
                    self.i.append(self.c[(r, c)])
                else:
                    self.c[(r, c)] = Cell(r, c)

        for i in range(self.d - 1):
            for s in range(self.d):
                self.c[(s, i)].n[3] = self.c[(s, i + 1)]
                self.c[(s, i + 1)].n[2] = self.c[(s, i)]
                self.c[(i, s)].n[1] = self.c[(i + 1, s)]
                self.c[(i + 1, s)].n[0] = self.c[(i, s)]

        iteration = -1
        while len(self.i) > 0 and iteration < 10:
            iteration += 1
            for cell in self.i:
                if self.__analyseCell(cell):
                    self.i.remove(cell)
                    print(self.i)

        # for r in range(self.d):
        #     for c in range(self.d):
        #         self.g.setCell(r, c, 0)

        return iteration

    def __setCell(self, row, col, val):
        self.g.setCell(row, col, val)
        self.c[(row, col)].t = val == True
        self.c[(row, col)].b = val == False

    def __analyseCell(self, cell):
        spaces, trans = zip(*([self.__travel(cell, i) for i in range(4)]))

        if cell.v == sum(spaces):
            for i, n in enumerate(spaces):
                next = cell.n[i] if n else None
                for _ in range(n):
                    if next.v == 0:
                        self.__setCell(next.row, next.col, 1)
                    next = next.n[i]
                if next:
                    self.__setCell(next.row, next.col, 0)
            return True

        index, single = 0, -1
        offset = [s - t for s, t in zip(spaces, trans)]
        for i, n in enumerate(offset):
            if n == sum(offset):
                index, single = i, n
        if single != -1:
            next = cell.n[index]
            for _ in range(cell.v - sum(trans)):
                self.__setCell(next.row, next.col, 1)
                next = next.n[index]
            if next:
                self.__setCell(next.row, next.col, 0)
            return True

        if cell.v == sum(trans):
            for i in range(4):
                blk = self.__end(cell, i)
                if blk is not None:
                    self.__setCell(blk.row, blk.col, 0)
            return True
        return False

    def __travel(self, c, d, s=-1, t=-1, n=True):
        if c:
            if not c.t:
                n = False
            if not c.b:
                return self.__travel(c.n[d], d, s + 1, t + n, n)
        return s, t

    def __end(self, c, d):
        if c is None:
            return None
        if c.b:
            return None
        if not c.v and not c.t:
            return c
        return self.__end(c.n[d], d)
