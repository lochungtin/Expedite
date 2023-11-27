from solvers.__solver import __Solver


class Solver(__Solver):
    def __init__(self, game):
        super().__init__(game)
        

    def run(self):
        print(self.g.guess([3, 5, 2, 3]))