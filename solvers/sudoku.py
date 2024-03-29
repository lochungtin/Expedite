from copy import deepcopy as dc

from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game):
        super().__init__(game)
        self.p = [[[True for _ in range(9)] for _ in range(9)] for _ in range(9)]
        self.s = [[False for _ in range(9)] for _ in range(9)]
        self.t = [[0 for _ in range(9)] for _ in range(9)]
        self.g = []

    def run(self):
        for r in range(9):
            for c in range(9):
                if self.b[r][c] != " ":
                    self.__set_board(r, c, int(self.b[r][c]) - 1)

        unsolved = True
        iterations = 0
        while unsolved:
            iterations += 1
            print(iterations)
            print([g[3] for g in self.g])
            if not self.__propagate():
                if len(self.g):
                    self.p, self.s, self.t, (r, c, n) = self.g.pop()
                    self.__set_board(r, c, n)
                else:
                    return None

            if self.__validation():
                for r, row in enumerate(self.t):
                    for c, cell in enumerate(row):
                        self.b[r][c] = cell
                        unsolved = False
            else:
                r, c, n = sorted(
                    [
                        (r, c, self.__poss_val(cell))
                        for r, row in enumerate(self.p)
                        for c, cell in enumerate(row)
                        if self.__poss_len(cell) > 1
                    ],
                    key=lambda x: len(x[2]),
                )[0]
                n = list(reversed(n))
                for i in range(1, len(n)):
                    self.g.append((dc(self.p), dc(self.s), dc(self.t), (r, c, n[i])))

                self.__set_board(r, c, n[0])

        return iterations

    def __propagate(self):
        changed = True
        while changed:
            changed = False
            for i in range(9):
                idxposs = [
                    list(zip(idx, [self.p[r][c] for (r, c) in idx]))
                    for idx in [self.__r_idx(i), self.__c_idx(i), self.__s_idx(i)]
                ]
                rSpans = dict([(i, set()) for i in range(9)])
                cSpans = dict([(i, set()) for i in range(9)])
                for (r, c), poss in idxposs[2]:
                    for p in range(9):
                        if not self.s[r][c]:
                            if poss[p]:
                                rSpans[p].add(r)
                                cSpans[p].add(c)
                for c, v in [(list(l)[0], i) for i, l in cSpans.items() if len(l) == 1]:
                    for r in range(9):
                        if r // 3 != i // 3 and not self.s[r][c] and self.p[r][c][v]:
                            self.p[r][c][v] = False
                            if self.__poss_len(self.p[r][c]) == 0:
                                return False
                            changed = True
                for r, v in [(list(l)[0], i) for i, l in rSpans.items() if len(l) == 1]:
                    for c in range(9):
                        if c // 3 != i % 3 and not self.s[r][c] and self.p[r][c][v]:
                            self.p[r][c][v] = False
                            if self.__poss_len(self.p[r][c]) == 0:
                                return False
                            changed = True
                for pair in idxposs:
                    len2 = list(filter(lambda x: self.__poss_len(x[1]) == 2, pair))
                    for a, (l0, p0) in enumerate(len2):
                        for b, (l1, p1) in enumerate(len2):
                            if all(x == y for x, y in zip(p0, p1)) and a < b:
                                for (r, c), _ in pair:
                                    if (r, c) != l0 and (r, c) != l1:
                                        for v in self.__poss_val(self.p[l0[0]][l0[1]]):
                                            if self.p[r][c][v]:
                                                self.p[r][c][v] = False
                                                if self.__poss_len(self.p[r][c]) == 0:
                                                    return False
                                                changed = True
                for pair in idxposs:
                    counts = dict([(i, []) for i in range(9)])
                    for (r, c), poss in pair:
                        if not self.s[r][c]:
                            for p in range(9):
                                if poss[p]:
                                    counts[p].append((r, c))
                    for v, l in counts.items():
                        if len(l) == 1:
                            self.__set_board(l[0][0], l[0][1], v)
                            changed = True
            if self.__fillSingle():
                changed = True
        if self.__fillSingle():
            changed = True
        return True

    def __set_board(self, r, c, v):
        if self.s[r][c]:
            return False
        idx = self.__r_idx(r) + self.__c_idx(c) + self.__s_idx((r // 3) * 3 + (c // 3))
        for i, j in list(set(idx)):
            self.p[i][j][v] = False
        self.p[r][c][v], self.s[r][c], self.t[r][c] = True, True, v + 1
        return True

    def __validation(self):
        for i in range(9):
            s = [self.__r_idx(i), self.__c_idx(i), self.__s_idx(i)]
            if any(map(lambda x: len(set([self.t[r][c] for (r, c) in x])) != 9, s)):
                return False
        return True

    def __fillSingle(self):
        changed = False
        for i in range(9):
            for j in range(9):
                if not self.s[i][j] and self.__poss_len(self.p[i][j]) == 1:
                    if self.__set_board(i, j, self.__poss_val(self.p[i][j])[0]):
                        changed = True
        return changed

    def __poss_len(self, p):
        return len(list(filter(lambda x: x, p)))

    def __poss_val(self, p):
        return [i for i in range(9) if p[i]]

    def __r_idx(self, r):
        return [(r, c) for c in range(9)]

    def __c_idx(self, c):
        return [(r, c) for r in range(9)]

    def __s_idx(self, s):
        r, c = (s // 3) * 3, (s % 3) * 3
        l = [((r, c + d), (r + 1, c + d), (r + 2, c + d)) for d in range(3)]
        return [p for r in l for p in r]
