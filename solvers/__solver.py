class __Solver:
    def __init__(self, game):
        self.g = game
        self.b = game.board
        self.d = game.size

    def run(self):
        raise NotImplementedError()
