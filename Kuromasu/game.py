from pathlib import Path


class Game:
    def __init__(self, file_index):
        path = Path("./Kuromasu/configs/config_{}.txt".format(file_index))
        with open(path, "r") as config:
            self.board = [list(line.strip().replace("-", " ")) for line in config.readlines()]
        self.size = len(self.board)

    def setCell(self, row, col, value):
        if self.board[row][col] == " ":
            self.board[row][col] = "-" if value else "x"
            return True
        return False

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        row = "+" + "".join("---+" for _ in range(self.size)) + "\n"
        rt = [row + "|" + "".join(f" {c} |" for c in l) for l in self.board]
        return "\n".join(rt + [row[0 : len(row) - 1]])
