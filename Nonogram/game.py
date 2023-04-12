class Game:
    def __init__(self, file_index):
        with open("configs/config_{}.txt".format(file_index), "r") as config:
            lines = config.readlines()
            self.size = int(lines[0])
            self.rCon = [c.strip().split(" ") for c in lines[1 : self.size + 1]]
            self.cCon = [c.strip().split(" ") for c in lines[self.size + 1 :]]
            self.board = [["-" for _ in range(self.size)] for _ in range(self.size)]

    def setCell(self, row, col, value):
        if self.board[row][col] == "-":
            self.board[row][col] = str(value * 1)
            return True
        return False

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        m = {"0": "░", "1": "▓", "-": " "}
        row = "+" + "".join("---+" for _ in range(self.size)) + "\n"
        rt = [row + "|" + "".join(" {} |".format(m[c]) for c in l) for l in self.board]
        return "\n".join(rt + [row[0 : len(row) - 1]])
