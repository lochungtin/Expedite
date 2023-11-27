from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.board = [list(l.strip().replace("-", " ")) for l in lines]
        self.size = len(self.board)

    def setCell(self, row, col, value):
        if self.board[row][col] == " ":
            self.board[row][col] = "-" if value else "x"
            return True
        return False
