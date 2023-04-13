class Solver:
    def __init__(self, game):
        self.g = game
        self.r = game.row
        self.c = game.col
        self.b = game.board
    
    def run(self):
        self.g.expand(self.r // 2, self.c // 2)

        iteration = -1
        while not self.g.validation() and iteration < 10:
            iteration += 1
            for r in range(self.r):
                for c in range(self.c):
                    val = self.b[r][c]
                    if val == " " or val == "x":
                        continue
                    neighbours = self.__neighbours(r, c)
                    empty, mines = self.__analyse(neighbours)
                    if val > 0 and len(empty) == val - mines:
                        for (er, ec) in empty:
                            self.g.flag(er, ec)

            for r in range(self.r):
                for c in range(self.c):
                    val = self.b[r][c]
                    if val == " " or val == "x":
                        continue
                    neighbours = self.__neighbours(r, c)
                    empty, mines = self.__analyse(neighbours)
                    if len(empty) > 0 and val == mines:
                        for (er, ec) in empty:
                            self.g.expand(er, ec)                    
        
        return iteration

    def __neighbours(self, r, c):
        shift = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
        return list(filter(
            lambda y: self.r > y[0] >= 0 and self.c > y[1] >= 0, 
            map(lambda x: (x[0] + r, x[1] + c), shift)))
    
    def __analyse(self, n):
        empty = list(filter(lambda x: self.b[x[0]][x[1]] == " ", n))
        mines = sum(map(lambda x: self.b[x[0]][x[1]] == "x", n))
        return empty, mines


