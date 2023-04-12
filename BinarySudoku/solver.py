class Solver:
    def __init__(self, game):
        self.g = game
        self.d = game.size
        self.c = [
            [{"0": 0, "1": 0, "-": 0} for _ in range(self.d)],
            [{"0": 0, "1": 0, "-": 0} for _ in range(self.d)],
        ]
        self.__maintain_counts()
        self.R = {
            "-00": (0, 1),
            "-11": (0, 0),
            "0-0": (1, 1),
            "1-1": (1, 0),
            "00-": (2, 1),
            "11-": (2, 0),
        }

    def run(self):
        iteration = -1
        while not self.__validation():
            iteration += 1
            for ori, cs in enumerate(self.c):
                for i, l in enumerate(self.__get_board(ori)):
                    for gi, g in enumerate(self.__grp_3(l)):
                        if g in self.R:
                            self.__set_cell(i, gi + self.R[g][0], self.R[g][1], ori)
                for l, c in enumerate(cs):
                    if c["0"] == self.d // 2 or c["1"] == self.d // 2:
                        for i in range(self.d):
                            self.__set_cell(l, i, c["0"] == self.d // 2, ori)
                for i, j in [(i, j) for i in range(self.d) for j in range(self.d)]:
                    if i != j:
                        if cs[i]["-"] == 0:
                            d = self.__calculate_difference(self.__get_board(ori), i, j)
                            if len(d) == 2:
                                self.__set_cell(j, d[0][0], d[0][1], ori)
                                self.__set_cell(j, d[1][0], d[1][1], ori)
        return iteration

    def __set_cell(self, row, col, value, ori):
        if self.g.setCell(*(col, row) if ori else (row, col), value):
            self.__maintain_counts()

    def __maintain_counts(self):
        for ori in range(2):
            self.c[ori] = [{"0": 0, "1": 0, "-": 0} for _ in range(self.d)]
            for il, l in enumerate(self.__get_board(ori)):
                for c in l:
                    self.c[ori][il][c] += 1

    def __get_board(self, ori):
        if ori:
            return [[self.g.board[i][j] for i in range(self.d)] for j in range(self.d)]
        return self.g.board

    def __grp_3(self, line):
        return ["".join(line[i : i + 3]) for i in range(self.d - 2)]

    def __calculate_difference(self, b, l, t):
        return [(i, not int(a)) for i, (a, b) in enumerate(zip(b[l], b[t])) if a != b]

    def __validation(self):
        for ori in range(2):
            b = self.__get_board(ori)
            if len(dict([("".join(l), None) for l in b])) != self.d:
                return False
            for l in b:
                if any("-" in g or all(g[0] == c for c in g) for g in self.__grp_3(l)):
                    return False
        if any(c["0"] != c["1"] for cs in self.c for c in cs):
            return False
        return True
