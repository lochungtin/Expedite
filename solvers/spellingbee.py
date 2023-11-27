from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game):
        super().__init__(game)
        with open("corpus.txt", "r") as f:
            self.c = [w.strip() for w in f.readlines() if len(w.strip()) > 3]
        self.m = game.center
        self.l = game.letters

    def run(self):
        s = set(self.l + [self.m])
        l1 = list(filter(lambda x: all(map(lambda y: y in s, x)) and self.m in x, self.c))
        l1.sort(key=len, reverse=True)
        self.g.board = l1
        return len(l1)
            
