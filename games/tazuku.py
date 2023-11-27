from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__(True)
        self.board = [list(l.strip()) for l in lines]
        self.size = len(self.board)

    def setCell(self, row, col, value):
        if self.board[row][col] == "-":
            self.board[row][col] = str(value * 1)
            return True
        return False
