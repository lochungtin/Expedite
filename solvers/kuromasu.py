from solvers.__solver import __Solver


class Cell:
    def __init__(self, row, col, val=0, trans=False, block=False):
        self.row = row
        self.col = col
        self.v = val
        self.t = trans
        self.b = block
        self.n = [None, None, None, None]


class Solver(__Solver):
    def run(self, game):
        super().__init__(game)
        self.i = []
        self.c = {}
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
        size = len(self.i) + 1
        while size > 0 and size != len(self.i):
            size = len(self.i)
            iteration += 1
            for cell in self.i:
                if self.__analyseCell(cell):
                    self.i.remove(cell)

        if len(self.i) == 0:
            for r in range(self.d):
                for c in range(self.d):
                    self.g.setCell(r, c, 0)

        return iteration

    def __setCell(self, row, col, val):
        if self.g.setCell(row, col, val):
            self.c[(row, col)].t = val == 1
            self.c[(row, col)].b = val == 0

    def __analyseCell(self, cell):
        _, trans = zip(*([self.__travel(cell, i) for i in range(4)]))
        for i in range(4):
            end = self.__end(cell, i)
            if end:
                end.t = True
                if cell.v < sum(trans) - trans[i] + self.__travel(cell, i)[1]:
                    self.__setCell(end.row, end.col, 0)
                end.t = False

        spaces, trans = zip(*([self.__travel(cell, i) for i in range(4)]))
        for i in range(4):
            rest = cell.v - sum(n for j, n in enumerate(spaces) if i != j)
            if rest > 0:
                next = cell.n[i]
                for _ in range(rest):
                    if next:
                        if next.v == 0:
                            self.__setCell(next.row, next.col, 1)
                        next = next.n[i]

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

        offset = [s - t for s, t in zip(spaces, trans)]
        single = max(offset) if max(offset) == sum(offset) else -1
        index = offset.index(single) if single != -1 else 0
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
        if c is None or c.b:
            return None
        if not c.v and not c.t:
            return c
        return self.__end(c.n[d], d)
