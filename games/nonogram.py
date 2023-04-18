from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__(True)
        self.size = int(lines[0])
        self.board = [["-" for _ in range(self.size)] for _ in range(self.size)]
        self.rCon = [c.strip().split(" ") for c in lines[1 : self.size + 1]]
        self.cCon = [c.strip().split(" ") for c in lines[self.size + 1 :]]

    def setCell(self, row, col, value):
        if self.board[row][col] == "-":
            self.board[row][col] = str(value * 1)
            return True
        return False
