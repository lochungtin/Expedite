from games.__game import __Game


class Game(__Game):
    def __init__(self, _):
        super().__init__(True)
        self.size = 4
        self._m = dict([(0, " ")] + [(2 ** (i + 1), chr(i + 97)) for i in range(17)])
        self.board = [[0 for _ in range(4)] for _ in range(4)]
