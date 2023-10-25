import re
from games.__game import __Game

class Game(__Game):
    def __init__(self, lines):
        super().__init__(True)
        self.size = int(lines[0])
        self.board = [["-" for _ in range(self.size)] for _ in range(self.size)]
        self.rCon = [list(map(int, c.strip().split(" "))) for c in lines[1 : self.size + 1]]
        self.cCon = [list(map(int, c.strip().split(" "))) for c in lines[self.size + 1 :]]
        self.reCon = list(map(self.__toRe, self.rCon + self.cCon))
        self.comp = [0 for _ in range(self.size * 2)]
        
    def __toRe(self, c):
        mid = "[0|-]+".join(["1{" + str(x) + "}" for x in c])
        return f"[0|-]*{mid}[0|-]*"
    
    def line(self, l):
        if l >= self.size:
            return [self.board[i][l % self.size] for i in range(self.size)]
        return [self.board[l][i] for i in range(self.size)]

    def setCell(self, row, col, value):
        if self.board[row][col] == "-":
            self.board[row][col] = str(value * 1)
            if re.match(self.reCon[row], "".join(self.line(row))):
                for i in range(self.size):
                    if self.board[row][i] == "-":
                        self.board[row][i] = "0"
                self.comp[row] = 1
            if re.match(self.reCon[self.size + col], "".join(self.line(self.size + col))):
                for i in range(self.size):
                    if self.board[i][col] == "-":
                        self.board[i][col] = "0"
                self.comp[self.size + col] = 1
            return True
        return False
