from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game):
        super().__init__(game)
        with open("corpus.txt", "r") as f:
            self.c = [w.strip() for w in f.readlines() if len(w.strip()) == 5]
        self.g = game
        self.l = "slate"
        self.h = {0: set(), 1: set(), 2: ["" for i in range(5)]}

    def run(self):
        iterations = -1
        while len(self.c) > 1:
            iterations += 1
            h = self.g.guess(self.l)
            if sum(h) == 10:
                return iterations + 1
            for i in range(5):
                if h[i] == 2:
                    self.h[2][i] = self.l[i]                
                else:
                    self.h[h[i]].add(self.l[i])
            self.c = list(filter(self.__valid, self.c))
            m = [[0 for _ in range(len(self.c))] for _ in range(len(self.c))]
            for i, w in enumerate(self.c):
                for j in range(i + 1, len(self.c)):
                    h = len(set(w)) + 10 - len(set(w).union(set(self.c[j])))
                    h += sum(a == b for a, b in zip(w, self.c[j]))
                    m[i][j] = 1 / h
                    m[j][i] = 1 / h
            self.l = sorted([(w, sum(r)) for w, r in zip(self.c, m)], key=lambda x: x[1])[0][0]
        if len(self.c) == 1:
            h = self.g.guess(self.c[0])
            return iterations + 1 if sum(h) == 10 else -1
        return -1

    def __valid(self, w):
        h0 = all(l not in w for l in list(self.h[0]))
        h1 = all(l in w for l in list(self.h[1]))
        h2 = all(m == "" or l == m for l, m in zip(w, self.h[2]))
        return h0 and h1 and h2
        