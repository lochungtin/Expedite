from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__(True)
        self.center = lines[0].strip()
        self.letters = lines[1].strip().split(" ")
        self.board = []

    def __repr__(self):
        if len(self.board) > 20:
            return "\n".join(self.board[0:20] + ["..."])
        return "\n".join(self.board)
