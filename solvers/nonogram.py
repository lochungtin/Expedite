from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game) -> None:
        super().__init__(game)
        self.c = game.rCon + game.cCon

    def run(self):
        iteration = -1
        while iteration < 1:
            iteration += 1
            for l, c in enumerate(self.c):
                if not self.g.comp[l]:
                    self.__overlapFill(l, c, self.__fs(l))
                    self.__condSumFill(l, c, self.__fs(l))
                    self.__extFill(l, c)
                    self.__sideFill(l, c)
        return iteration

    def __lFill(self, l, i, v=1):
        if l >= self.d:
            l, i = i, l % self.d
        self.g.setCell(l, i, v)

    def __lRangeFill(self, l, s, n, v=1):
        [self.__lFill(l, i, v) for i in range(max(s, 0), min(s + n, self.d))]

    def __fs(self, l):
        a = list(map(lambda x: x == "0", self.g.line(l)))
        return a.index(False), self.d - list(reversed(a)).index(False)

    def __overlapFill(self, l, c, f):
        if len(c) == 1:
            self.__lRangeFill(l, f[1] - c[0], c[0] * 2 - f[1] + f[0])

    def __condSumFill(self, l, c, f):
        if len(c) + sum(c) - 1 == f[1] - f[0]:
            idx = f[0]
            for p in c:
                self.__lRangeFill(l, idx, p)
                idx += p + 1
        
    def __extFill(self, l, c):
        if len(c) == 1:
            a = self.g.line(l)
            if "1" in a:
                le1, ri1 = a.index("1"), self.d - list(reversed(a)).index("1") - 1
                self.__lRangeFill(l, le1, ri1 - le1)
                le0, ri0 = 0, 10
                if "0" in a[0:le1]:
                    le0 = le1 - list(reversed(a[0:le1])).index("0") - 1
                if "0" in a[ri1:10]:
                    ri0 = ri1 + a[ri1:10].index("0")
                self.__lRangeFill(l, ri1 + 1, c[0] - ri1 + le0)
                self.__lRangeFill(l, ri0 - le1, le1 + c[0] - ri0)

    def __sideFill(self, l, c):
        a = self.g.line(l)
        if a[0] == "1":
            self.__lRangeFill(l, 0, c[0])
        if a[-1] == "1":
            self.__lRangeFill(l, self.d - c[-1], c[-1])