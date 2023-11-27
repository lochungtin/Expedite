from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.sides = [line.strip().split(" ") for line in lines]
        self.board = [f"[{line.strip()}]" for line in lines]

    def __repr__(self):
        return "-".join(self.board)
