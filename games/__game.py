class __Game:
    def __init__(self, binaryDisplay=False):
        self.board, self.size = None, None
        self.bd = binaryDisplay

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        m = {"0": "░", "1": "▓", "-": " "}
        row = "+" + "".join("---+" for _ in range(len(self.board))) + "\n"
        rt = [
            row + "|" + "".join(" {} |".format(m[c] if self.bd else c) for c in l)
            for l in self.board
        ]
        return "\n".join(rt + [row[0 : len(row) - 1]])
