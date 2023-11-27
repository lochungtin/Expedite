from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game):
        super().__init__(game)
        with open("corpus.txt", "r") as f:
            self.c = [w.strip() for w in f.readlines() if len(w.strip()) > 3]
        self.s = game.sides
        self.f = set([l for w in self.s for l in w])
        self.n = dict((w, i) for i in range(4) for w in self.s[i])

    def run(self):
        l1 = list(filter(lambda x: all(map(lambda y: y in self.f, x)), self.c))
        print(len(l1))
        l2 = list(filter(self.__notAdj, l1))
        print(len(l2))
        l2.sort(key=lambda x: len(set(x)), reverse=True)
        print(l2)
        return -1

    def __notAdj(self, w):
        return all(self.n[w[i]] != self.n[w[i+1]] for i in range(len(w) - 1))
