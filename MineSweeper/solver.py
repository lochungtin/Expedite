from random import shuffle


class Solver:
    def __init__(self, game):
        self.g = game
        self.r = game.row
        self.c = game.col
        self.b = game.board
        self.l = [(r, c) for r in range(self.r) for c in range(self.c)]
    
    def run(self):
        self.g.expand(self.r // 2, self.c // 2)
        iteration = -1
        guesses = 0
        deaths = 0
        while not self.g.validation():
            iteration += 1
            modified = False
            for (r, c) in self.l:
                bypass, val, empty, mines = self.__analyse(r, c)
                if bypass:
                    continue
                if val > 0 and len(empty) == val - mines:
                    for (er, ec) in empty:
                        modified = True
                        self.g.flag(er, ec)

            for (r, c) in self.l:
                bypass, val, empty, mines = self.__analyse(r, c)
                if bypass:
                    continue
                if len(empty) > 0 and val == mines:
                    for (er, ec) in empty:
                        modified = True
                        self.g.expand(er, ec)                    
        
            if not modified:
                guesses += 1
                empty = list(filter(lambda x: self.b[x[0]][x[1]] == " ", self.l))
                shuffle(empty)
                selected = self.g.expand(*(empty[0]))
                while selected:
                    deaths += 1
                    empty = empty[1:]
                    selected = self.g.expand(*(empty[0]))

        return iteration, guesses, deaths
    
    def __analyse(self, r, c):
        val = self.b[r][c]
        if val == " " or val == "x":
            return True, 0, 0, 0
        shift = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
        locs = map(lambda x: (x[0] + r, x[1] + c), shift)
        n = list(filter(lambda y: self.r > y[0] >= 0 and self.c > y[1] >= 0, locs))
        empty = list(filter(lambda x: self.b[x[0]][x[1]] == " ", n))
        return False, val, empty, sum(map(lambda x: self.b[x[0]][x[1]] == "x", n))
