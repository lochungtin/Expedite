from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game) -> None:
        super().__init__(game)
        self.c = [list(map(int, c)) for c in (game.rCon + game.cCon)]
        self.e = [[(0, self.d)] for _ in range(self.d * 2)]

    def lFill(self, l, i, v):
        if l >= self.d:
            l, i = i, l % self.d
        self.g.setCell(l, i, v)

    def lRangeFill(self, l, v, s, n):
        [self.lFill(l, i, v) for i in range(s, s + n)]

    def halfWidthMinFill(self, l, e, n):
        t = n * 2 - e[1]
        self.lRangeFill(l, 1, n - t + e[0], t)

    def conMaxFill(self, l, e, c):
        if sum(c) + len(c) == e[1] + 1:
            loc = list(map(int, " 1 ".join(map(str, c)).split(" ")))
            for i, s in enumerate(loc):
                self.lRangeFill(l, (i + 1) % 2, e[0] + sum(loc[0:i]), s)

    def run(self):
        self.conMaxFill(0, (0, 10), [1, 2, 2, 2])
