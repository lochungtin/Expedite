from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.answer = list(map(int, lines[0].strip().split(" ")))
        self.aFreq = [self.answer.count(i) for i in range(6)]
        self.board = [[0, 0, 0, 0]]
        self.size = 1

    def guess(self, g):
        self.board[0] = g
        b = sum([a == b for a, b in zip(g, self.answer)])
        f = [g.count(i) for i in range(6)]
        w = sum([min(a, b) for a, b in zip(f, self.aFreq)])
        return b, w
